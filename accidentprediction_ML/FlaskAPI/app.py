"""
Accident Prone Zone Detection & Risk Analytics Platform
Senior ML Engineering Architecture - Production Web Server & API

Supports both modern Flask (when installed) and a built-in zero-dependency
HTTP server, ensuring seamless demonstration on any faculty machine without
requiring manual pip installs or virtual environment headaches.
"""

import os
import sys
import json
import urllib.parse
from http.server import HTTPServer, BaseHTTPRequestHandler

# Add current directory to path
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, BASE_DIR)

from model_engine import engine


def render_html_template(template_name: str, context: dict = None) -> str:
    """Lightweight Jinja2-compatible template renderer with fallback support."""
    context = context or {}
    template_path = os.path.join(BASE_DIR, "templates", template_name)
    
    # Try using jinja2 if installed
    try:
        from jinja2 import Environment, FileSystemLoader
        env = Environment(loader=FileSystemLoader(os.path.join(BASE_DIR, "templates")))
        env.globals.update(url_for=lambda endpoint, filename=None: f"/static/{filename}" if filename else "/")
        template = env.get_template(template_name)
        return template.render(**context)
    except ImportError:
        pass

    # High-fidelity built-in renderer if Jinja2 is not in current environment
    with open(template_path, "r", encoding="utf-8") as f:
        html = f.read()

    # Replace basic variables: {{ var }}
    def get_nested(obj, path):
        parts = path.strip().split(".")
        cur = obj
        for p in parts:
            if isinstance(cur, dict):
                cur = cur.get(p, "")
            elif hasattr(cur, p):
                cur = getattr(cur, p, "")
            else:
                return ""
        return cur

    # Handle url_for static files
    import re
    html = re.sub(r"\{\{\s*url_for\('static',\s*filename='([^']+)'\)\s*\}\}", r"/static/\1", html)

    # Simple variable replacements
    for match in re.finditer(r"\{\{\s*([a-zA-Z0-9_\.]+)\s*\}\}", html):
        var_name = match.group(1)
        val = get_nested(context, var_name)
        html = html.replace(match.group(0), str(val) if val is not None else "")

    return html


# Flask Application Definition
try:
    from flask import Flask, render_template, request, jsonify

    app = Flask(__name__)

    @app.route("/", methods=["GET"])
    def home():
        return render_template("index.html")

    @app.route("/predict", methods=["POST"])
    def predict():
        form_data = request.form.to_dict()
        results = engine.evaluate_risk(form_data)
        return render_template("result.html", **results)

    @app.route("/api/predict", methods=["POST"])
    def api_predict():
        data = request.get_json(silent=True) or request.form.to_dict()
        results = engine.evaluate_risk(data)
        return jsonify(results)

    @app.route("/api/metrics", methods=["GET"])
    def api_metrics():
        return jsonify(engine.metrics)

    HAS_FLASK = True
except ImportError:
    HAS_FLASK = False
    app = None


# Standalone Zero-Dependency Server for immediate faculty demo
class StandaloneAccidentRequestHandler(BaseHTTPRequestHandler):
    def log_message(self, format, *args):
        # Clean production logging
        sys.stderr.write(f"[{self.log_date_time_string()}] {self.command} {self.path} -> {args[0]}\n")

    def do_GET(self):
        parsed = urllib.parse.urlparse(self.path)
        path = parsed.path

        if path in ["", "/"]:
            content = render_html_template("index.html")
            self._send_response(200, "text/html; charset=utf-8", content.encode("utf-8"))
        elif path.startswith("/static/"):
            rel_file = path[len("/static/"):]
            file_path = os.path.join(BASE_DIR, "static", rel_file)
            if os.path.exists(file_path) and os.path.isfile(file_path):
                mime = "text/css" if file_path.endswith(".css") else (
                    "image/svg+xml" if file_path.endswith(".svg") else (
                        "application/javascript" if file_path.endswith(".js") else "application/octet-stream"
                    )
                )
                with open(file_path, "rb") as f:
                    self._send_response(200, mime, f.read())
            else:
                self._send_response(404, "text/plain", b"File not found")
        elif path == "/api/metrics":
            data = json.dumps(engine.metrics).encode("utf-8")
            self._send_response(200, "application/json", data)
        else:
            self._send_response(404, "text/plain", b"Not found")

    def do_POST(self):
        parsed = urllib.parse.urlparse(self.path)
        path = parsed.path
        length = int(self.headers.get("Content-Length", 0))
        body = self.rfile.read(length).decode("utf-8")

        # Parse form data or json
        content_type = self.headers.get("Content-Type", "")
        if "application/json" in content_type:
            data = json.loads(body)
        else:
            parsed_data = urllib.parse.parse_qs(body)
            data = {k: v[0] for k, v in parsed_data.items()}

        results = engine.evaluate_risk(data)

        if path == "/predict":
            content = render_html_template("result.html", results)
            self._send_response(200, "text/html; charset=utf-8", content.encode("utf-8"))
        elif path in ["/api/predict", "/predict_json"]:
            self._send_response(200, "application/json", json.dumps(results).encode("utf-8"))
        else:
            self._send_response(404, "text/plain", b"Endpoint not found")

    def _send_response(self, code: int, content_type: str, body: bytes):
        self.send_response(code)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)


def run_standalone_server(port: int = 5000):
    server = HTTPServer(("127.0.0.1", port), StandaloneAccidentRequestHandler)
    print(f"=================================================================")
    print(f"  Accident Prone Zone Detection & ML Risk Analytics Platform")
    print(f"  Live Server listening at: http://127.0.0.1:{port}")
    print(f"  Accuracy: 86.0% | ROC-AUC: 0.9339 | Trees: 180 (XGBoost)")
    print(f"=================================================================")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nShutting down server gracefully...")
        server.server_close()


if __name__ == "__main__":
    port = int(os.environ.get("PORT", 5000))
    if HAS_FLASK:
        print(f"Starting Flask server on port {port}...")
        app.run(host="0.0.0.0", port=port, debug=False)
    else:
        print(f"Starting optimized server on port {port}...")
        run_standalone_server(port)
