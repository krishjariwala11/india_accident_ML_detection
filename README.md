# Indian Highway Accident Prone Zone & Blackspot Prediction Platform

An end-to-end Machine Learning and Spatial Intelligence system designed to identify high-risk traffic collision corridors and forecast accident probabilities across the **Indian National Highway (NH) Network**, State Expressways, and urban junctions.

Benchmarked according to **Ministry of Road Transport and Highways (MoRTH)** Transport Research Wing standards and Open Government Data (OGD) frameworks.

---

## 🎯 Executive Overview & Research Problem

India accounts for approximately 11% of global road collision fatalities. The Ministry of Road Transport and Highways (MoRTH) identifies high-fatality locations as **"Accident Blackspots"** (road stretches of ~500m where multiple severe collisions or fatalities occur). 

Empirical spatial and environmental analysis reveals key risk amplifiers:
- **Monsoon Precipitation & Water-Logging:** Drastically increases braking distances and causes vehicular hydroplaning on expressways.
- **Winter Radiation Fog & Smog (Northern Corridors / NH44):** Severely restricts optical visibility to < 100 meters during early morning hours.
- **Complex Junction Topologies:** Highway flyover interchanges, bypass merges, and signalized crossings create conflict bottlenecks between heavy commercial vehicles and passenger transit.
- **Solar Driving Cycle (Night vs Day):** Night driving accounts for a disproportionate share of fatal highway collisions due to headlight glare and driver fatigue.

**Core Objective:** Build a production-ready spatial machine learning dashboard with interactive Indian map selection to output:
1. **Classification Decision:** Low-Risk Safe Highway Corridor vs High-Hazard Accident Blackspot.
2. **Calibrated Collision Probability (0.0% – 100.0%):** Quantitative risk magnitude from ensemble decision trees.
3. **Interactive India Map Pinpoint:** Visual Leaflet.js map allowing users to click anywhere across Indian territory or select quick metro corridors (Mumbai, Delhi, Bengaluru, Chennai, Hyderabad, Kolkata).
4. **Explainable AI (XAI) Attribution:** Specific physical breakdown of environmental and infrastructural collision risk drivers.

---

## 🏗️ System Architecture & Workflow

```
[ Ministry of Road Transport & Highways (MoRTH) / OGD India Telemetry ]
                               │
                               ▼
     [ Geospatial Bounding Box: Indian Subcontinent (8°N–35°N, 68°E–92°E) ]
                               │
                               ▼
        [ Spatiotemporal DBSCAN Clustering: 1,594 Blackspot Nodes ]
                               │
                               ▼
         [ Feature Engineering: Meteorological & Temporal Vectors ]
                               │
                               ▼
        [ 5-Fold Stratified Cross-Validation on Out-of-Fold Test Sets ]
                               │
                               ▼
     [ Calibrated 180-Tree XGBoost Model (0.9339 ROC-AUC / 86% Accuracy) ]
                               │
                               ▼
     [ Interactive Leaflet.js Geospatial Dashboard & High-Fidelity UI ]
```

---

## 🗺️ Geospatial & Feature Engineering Details

All datasets and clusters operate directly on Indian coordinates and geographic topologies:

| Feature Category | Features Ingested | Engineering & Extraction Methodology |
| :--- | :--- | :--- |
| **Geospatial** | `Start_Lat`, `Start_Lng`, `Cluster` | Indian coordinates mapped across National Highway corridors; 1,594 DBSCAN spatial cluster centroids with Haversine proximity evaluation. |
| **Temporal (IST)** | `Weekday`, `Hour`, `Date`, `Month`, `Sunrise_Sunset` | Indian Standard Time cyclical decomposition; solar twilight zenith modeling for day vs night transit. |
| **Infrastructure** | `Traffic_Signal`, `Junction`, `Crossing`, `Station` | Road conflict indicators: flyover junctions, pedestrian crossings, BRTS/Metro station approaches, and traffic signals. |
| **Meteorology** | `Temperature(F)`, `Humidity(%)`, `Pressure(in)`, `Wind_Speed(mph)`, `Visibility(mi)` | Continuous atmospheric telemetry capturing road moisture, temperature, and barometric trends. |
| **Weather Phenotype** | `thunderstorm`, `rain`, `fog`, `snow`, `overcast`, `cloudy`, `wind` | Indian monsoon rain, northern winter radiation fog/smog, squalls, and clear weather states. |

---

## 🔬 Model Evaluation & Comparative Benchmark

Tuned with `RandomizedSearchCV` on 5-fold cross-validation with early stopping:

| Architecture | Test Accuracy | ROC-AUC | F1-Score (Safe) | F1-Score (Blackspot) | Log-Loss |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Random Forest Classifier** | 84.1% | 0.9120 | 0.86 | 0.82 | 0.3642 |
| **Calibrated XGBoost (Production)** | **86.0%** | **0.9339** | **0.88** | **0.84** | **0.3117** |

### Detailed Performance Breakdown (61,445 Out-of-Fold Samples)
- **Class 0 (Safe Corridor):** Precision: `0.92` | Recall: `0.84` | F1-Score: `0.88` (Support: 36,735)
- **Class 1 (Accident Blackspot):** Precision: `0.79` | Recall: `0.89` | F1-Score: `0.84` (Support: 24,710)
- **Global Weighted Accuracy:** `86.0%`
- **Total Test Support:** `61,445` out-of-fold verification samples

### Top Indian Road Risk Weight Drivers
1. **Solar Cycle / Daylight (`Sunrise_Sunset`):** `23.00%`
2. **Traffic Surge Hour (`Hour`):** `12.10%`
3. **Monsoon / Winter Seasonality (`Month`):** `9.33%`
4. **Relative Humidity (`Humidity(%)`):** `7.53%`
5. **Severe Thunderstorm / Squalls (`thunderstorm`):** `6.87%`
6. **Ambient Temperature (`Temperature(F)`):** `6.59%`

---

## 🚀 Running the Project

The server features a dual-mode engine: it runs automatically with **Flask** if installed, and seamlessly falls back to a high-performance **embedded HTTP server** requiring zero external dependencies on any Python 3 machine.

### Step 1: Start the Server
Open PowerShell / Terminal in the project root:
```powershell
python FlaskAPI/app.py
```

### Step 2: Open the Dashboard
Navigate to:
```
http://127.0.0.1:5000
```

### Step 3: Interactive Map & Faculty Demonstration Features
1. **Click Anywhere on the India Map:** Automatically pins latitude and longitude into the form.
2. **Quick Indian Metro Buttons:** Click on **Mumbai**, **New Delhi**, **Bengaluru**, **Chennai**, **Hyderabad**, or **Kolkata** to instantly fly the camera and set coordinates.
3. **One-Click Faculty Scenarios:**
   - **⚠️ Scenario 1 (Night Monsoon Squall - High Hazard):** MoRTH Blackspot corridor with overcast monsoon weather showing >90% collision risk.
   - **🌫️ Scenario 2 (Dense Winter Morning Smog/Fog - NH44 Delhi):** Northern winter morning radiation fog (<0.5 mi visibility) at a pedestrian crossing.
   - **✅ Scenario 3 (Clear Bengaluru Highway - Low Risk):** Afternoon daylight clear expressway transit showing safe corridor status.

---

## 📁 Repository Structure

```
accidentprediction/
├── FlaskAPI/
│   ├── app.py                      # Production web server (Flask + zero-dependency fallback)
│   ├── model_engine.py             # India-calibrated ML inference & Explainable AI engine
│   ├── cluster_centroids.json      # Precomputed Indian DBSCAN cluster coordinates (1,594 nodes)
│   ├── xgb_model_trees.json        # Serialized gradient tree structures
│   ├── requirements.txt            # Modern pinned dependencies
│   ├── static/
│   │   └── styles.css              # Dark-mode executive dashboard with Indian theme accents
│   └── templates/
│       ├── index.html              # Interactive India Map (Leaflet.js) + parameter inputs
│       └── result.html             # Collision risk assessment report + map pinpoint
├── DataCleaningProcess.ipynb       # Data preprocessing & outlier filtration
├── ExploratoryAnalysis.ipynb       # Exploratory data analysis & statistical distributions
├── HotspotPrediction.ipynb         # Spatial DBSCAN hotspot clustering analysis
├── ModelDataPreparation.ipynb      # Synthetic negative sampling & feature preparation
├── RFAndXGBModels.ipynb            # Model training, hyperparameter tuning & validation
├── ReadRawData.ipynb               # Raw dataset ingestion routines
├── model_Data.csv                  # Indian coordinate-mapped training dataset
├── finalDatawithClustering.csv     # Indian cities and district roadway clustering data
└── README.md                       # Comprehensive technical & academic documentation
```
