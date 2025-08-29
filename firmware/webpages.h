#pragma once

/*
// Neutral WiFi Logo
const char svg_logo[] PROGMEM = R"rawliteral(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 200 200">
    <circle cx="100" cy="100" r="80" fill="#3b82f6"/>
    <text x="100" y="115" font-size="45" text-anchor="middle" fill="white">WiFi</text>
</svg>
)rawliteral";
*/

// HF-ICT Logo
const char svg_logo[] PROGMEM = R"rawliteral(
  <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1303 697" version="1.1">
    <path d="M 0 369.500 L 0 568 29 568 L 58 568 58 454.500 L 58 341 86 341 L 114 341 114 454.500 L 114 568 142.500 568 L 171 568 171 454.500 L 171 341 142.500 341 L 114 341 114 312.500 L 114 284 86 284 L 58 284 58 227.500 L 58 171 29 171 L 0 171 0 369.500 M 284 199.500 L 284 228 255.500 228 L 227 228 227 398 L 227 568 255.496 568 L 283.991 568 284.246 454.750 L 284.500 341.500 312.750 341.234 L 341 340.969 341 312.484 L 341 284 312.500 284 L 284 284 284 256 L 284 228 312.500 228 L 341 228 341 199.500 L 341 171 312.500 171 L 284 171 284 199.500" stroke="none" fill="#3682B0" fill-rule="evenodd"/><path d="M 1215 0.655 C 1188.079 4.293, 388.080 128.350, 384.440 129.452 C 368.288 134.338, 358.542 156.457, 361.946 180.500 C 362.569 184.900, 372.405 222.925, 383.803 265 L 404.526 341.500 429.263 341.767 L 454 342.035 454 369.517 L 454 397 437 397 C 427.650 397, 420 397.400, 420 397.888 C 420 400.356, 461.131 550.356, 462.585 553.191 C 468.376 564.486, 478.442 574.883, 488.914 580.385 C 496.139 584.182, 498.148 584.587, 785.844 640.268 L 1075.500 696.329 1090.500 696.327 C 1102.703 696.326, 1106.952 695.928, 1113.287 694.194 C 1129.528 689.749, 1144.111 679.545, 1149.593 668.789 C 1152.964 662.176, 1300.576 91.152, 1302.051 79.019 C 1304.608 57.993, 1298.598 38.321, 1285.196 23.845 C 1271.460 9.009, 1253.444 1.487, 1229.500 0.593 C 1223.450 0.367, 1216.925 0.395, 1215 0.655 M 1024 170.556 L 1024 227.111 1053.250 226.806 C 1080.661 226.519, 1082.791 226.366, 1087.122 224.363 C 1098 219.333, 1103.922 210.281, 1104.775 197.379 C 1105.559 185.539, 1101.301 176.717, 1092.149 171.214 C 1089.095 169.377, 1088.979 169.142, 1090.837 168.552 C 1093.712 167.639, 1098.922 161.424, 1101.316 156.051 C 1102.663 153.028, 1103.329 149.448, 1103.299 145.389 C 1103.173 128.440, 1093.216 117.569, 1075.500 115.039 C 1071.650 114.489, 1058.487 114.031, 1046.250 114.020 L 1024 114 1024 170.556 M 1125 170.500 L 1125 227 1161.500 227 L 1198 227 1198 218.017 L 1198 209.034 1172.250 208.767 L 1146.500 208.500 1146.240 161.250 L 1145.980 114 1135.490 114 L 1125 114 1125 170.500 M 1045 147.128 L 1045 161.256 1058.750 160.802 C 1074.046 160.297, 1077.857 159.110, 1080.483 154.033 C 1082.444 150.241, 1082.421 143.715, 1080.434 139.872 C 1077.730 134.643, 1072.446 133.047, 1057.750 133.022 L 1045 133 1045 147.128 M 511 199.500 L 511 227 539 227 L 567 227 567 199.500 L 567 172 539 172 L 511 172 511 199.500 M 1045 194.626 L 1045 209.252 1059.250 208.805 C 1074.668 208.320, 1078.488 207.238, 1081.615 202.465 C 1085.790 196.093, 1083.857 185.712, 1077.810 182.026 C 1074.886 180.243, 1072.780 180.007, 1059.750 180.004 L 1045 180 1045 194.626 M 255.484 227.742 L 283.969 228.031 284.259 256.265 L 284.548 284.500 284.524 256 L 284.500 227.500 255.750 227.477 L 226.999 227.453 255.484 227.742 M 852 369.500 L 852 511 879.500 511 L 907 511 907 426 L 907 341 935.500 341 L 964 341 964 313 L 964 285 935.500 285 L 907 285 907 256.500 L 907 228 879.500 228 L 852 228 852 369.500 M 114.234 312.234 L 114 340.969 85.750 341.251 L 57.500 341.532 85.781 341.766 C 107.818 341.948, 114.170 341.721, 114.548 340.737 C 114.814 340.042, 114.905 326.879, 114.750 311.487 L 114.467 283.500 114.234 312.234 M 511 426.500 L 511 568 539 568 L 567 568 567 426.500 L 567 285 539 285 L 511 285 511 426.500 M 682 313 L 682 341 738 341 L 794 341 794 313 L 794 285 738 285 L 682 285 682 313 M 284.667 341.667 C 284.300 342.033, 284 393.101, 284 455.151 L 284 567.969 255.250 568.258 L 226.500 568.548 255.500 568.524 L 284.500 568.500 284.754 455.266 L 285.009 342.031 313.254 341.749 L 341.500 341.468 313.417 341.234 C 297.971 341.105, 285.033 341.300, 284.667 341.667 M 625 426.500 L 625 511 653 511 L 681 511 681 426.500 L 681 342 653 342 L 625 342 625 426.500 M 682 540 L 682 568 738 568 L 794 568 794 540 L 794 512 738 512 L 682 512 682 540 M 908 540 L 908 568 936 568 L 964 568 964 540 L 964 512 936 512 L 908 512 908 540 M 14.250 568.748 C 22.362 568.916, 35.637 568.916, 43.750 568.748 C 51.862 568.580, 45.225 568.442, 29 568.442 C 12.775 568.442, 6.137 568.580, 14.250 568.748 M 128.311 568.748 C 136.457 568.916, 149.507 568.916, 157.311 568.747 C 165.115 568.579, 158.450 568.441, 142.500 568.441 C 126.550 568.442, 120.165 568.580, 128.311 568.748" stroke="none" fill="#148c3c" fill-rule="evenodd"/>
  </svg>
)rawliteral";

// Captive Portal
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>WiFi Setup</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background:#f3f4f6;
      margin:0;
      padding:0;
      min-height:100vh;
    }
    .container {
      max-width:400px;
      margin:40px auto 80px; /* unten etwas mehr Platz wegen Footer */
      padding:40px;
      background:#fff;
      border-radius:16px;
      box-shadow:0 4px 10px rgba(0,0,0,0.1);
    }
    .logo {
      display:block;
      margin:0 auto 40px;
      width:100%;
      height:auto;
    }
    h2 {
      text-align:center;
      color:#111827;
    }
    form {
      margin-bottom: 10px;
    }
    label {
      display:block;
      margin-top:15px;
      margin-bottom:5px;
      font-weight:600;
      color:#374151;
      text-align:left;
    }
    select,
    input[type=password],
    button {
      display:block;
      width:100%;
      margin-top:8px;
      padding:10px;
      border:1px solid #d1d5db;
      border-radius:8px;
      font-size:16px;
      box-sizing:border-box;
    }
    select {
      appearance:none;
      -webkit-appearance:none;
      -moz-appearance:none;
      background:#fff url('data:image/svg+xml;utf8,<svg xmlns="http://www.w3.org/2000/svg" width="14" height="14" fill="none" stroke="%23374151" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="2 5 7 10 12 5"/></svg>') no-repeat right 12px center;
      background-size:14px;
      padding-right:32px;
      cursor:pointer;
    }
    button {
      margin-top:20px;
      background:#3b82f6;
      color:#fff;
      font-weight:600;
      border:none;
      cursor:pointer;
      padding:12px;
    }
    .footer {
      position:fixed;
      bottom:0;
      left:0;
      right:0;
      text-align:center;
      padding:10px;
      font-size:14px;
      color:#6b7280; /* dezentes Grau im Lightmode */
      background:transparent;
    }
    .footer .heart {
      color:#e11d48; /* Rot fürs Herz */
    }
    @media (prefers-color-scheme: dark){
      body{background:#111827;color:#f9fafb;}
      .container{background:#1f2937;}
      h2{color:#f9fafb;}
      label{color:#d1d5db;}
      select,
      input[type=password]{
        background:#111827;
        color:#f9fafb;
        border:1px solid #374151;
      }
      select {
        background:#111827 url('data:image/svg+xml;utf8,<svg xmlns="http://www.w3.org/2000/svg" width="14" height="14" fill="none" stroke="%23d1d5db" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="2 5 7 10 12 5"/></svg>') no-repeat right 12px center;
        background-size:14px;
      }
      button{background:#2563eb;}
      .footer {color:#9ca3af;} /* helleres Grau im Darkmode */
    }
  </style>
</head>
<body>
  <div class="container">
    <!-- Logo -->
    <div class="logo">
      %SVG_LOGO%
    </div>

    <h2>WLAN konfigurieren</h2>
    <form action="/save" method="POST">
      <label for="ssid">SSID auswählen</label>
      <select name="ssid" id="ssid">
        %SSID_OPTIONS%
      </select>
      <label for="pass">Passwort</label>
      <input type="password" id="pass" name="pass" placeholder="WLAN Passwort">
      <button type="submit">💾 Speichern</button>
    </form>
  </div>

  <div class="footer">
    Made with <span class="heart">♥</span> by Severin Holm (2025 hf-ict - IoT)
  </div>
</body>
</html>
)rawliteral";


// Captive Portal
const char saved_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>WiFi Setup – Gespeichert</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background:#f3f4f6;
      margin:0;
      padding:0;
      min-height:100vh;
    }
    .container {
      max-width:400px;
      margin:40px auto 80px;
      padding:40px;
      background:#fff;
      border-radius:16px;
      box-shadow:0 4px 10px rgba(0,0,0,0.1);
      text-align:center;
    }
    h2 {
      color:#111827;
      margin-bottom:20px;
    }
    .success-icon {
      font-size:50px;
      color:#10b981; /* grün */
      margin-bottom:20px;
    }
    p {
      color:#374151;
      font-size:16px;
      margin-bottom:20px;
    }
    .footer {
      position:fixed;
      bottom:0;
      left:0;
      right:0;
      text-align:center;
      padding:10px;
      font-size:14px;
      color:#6b7280;
      background:transparent;
    }
    .footer .heart {color:#e11d48;}
    @media (prefers-color-scheme: dark){
      body{background:#111827;color:#f9fafb;}
      .container{background:#1f2937;}
      h2{color:#f9fafb;}
      p{color:#d1d5db;}
      .footer{color:#9ca3af;}
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="success-icon">✅</div>
    <h2>Daten gespeichert</h2>
    <p>Der ESP32 startet nun neu und verbindet sich mit deinem WLAN.</p>
  </div>
  <div class="footer">
    Made with <span class="heart">♥</span> by Severin Holm (2025 hf-ict - IoT)
  </div>
</body>
</html>
)rawliteral";