document.addEventListener('DOMContentLoaded', () => {
  const backButton = document.getElementById('backButton');
  const nextButton = document.getElementById('nextButton');
  const stepContent = document.getElementById('stepContent');
  const pageIndicator = document.getElementById('pageIndicator');
  const totalSteps = 6;
  let currentStep = 0;
  let selectedDeviceIndex = 0;
  let startIndex = 0;

  const devices = [
    { id: "switcher", name: "IoT Switcher", version: "1.2.0", folder: "iot_switcher", ap_ssid: "⚡ IoT-Switcher Setup ⚡" },
    { id: "sensor",  name: "IoT Sensor",    version: "0.9.5", folder: "iot_sensor",   ap_ssid: "⚡ IoT-Sensor Setup ⚡" },
    { id: "light",   name: "IoT Light Controller", version: "2.0.1", folder: "iot_light",  ap_ssid: "⚡ IoT-Light Setup ⚡" },
    { id: "gateway", name: "IoT Gateway",   version: "1.0.0", folder: "iot_gateway",  ap_ssid: "⚡ IoT-Gateway Setup ⚡" }
  ];

  const steps = [
    {
      title: null,
      description: null,
      content: () => `
        <div class="logo"><img src="assets/img/logo.png" alt="Logo"></div>
        <p class="step-description"><strong>ESP Installer Version: 1.0.0 Beta</strong><br>
        Diese Seite gehört zum Modul IoT an der Höheren Fachschule für Informatik (hf-ict). 
        Der Assistent dient dem Flashen der passenden ESP32-Firmware. Außerdem unterstützt er 
        dabei, das WLAN des ESP32 korrekt einzurichten und den WebClient auf der richtigen Geräte-IP zu finden.</p>
        <button id="startAssistantBtn" class="styledBtn" style="margin-top:40px;">Installation Assistent starten</button>
      `
    },
    {
      title: "Welches Gerät willst du installieren?",
      description: "Wähle dein Gerät aus der Liste unten, um fortzufahren.",
      content: () => `
        <div class="device-carousel-wrapper" id="carouselWrapper"></div>
        <div class="carousel-controls">
          <button class="carousel-btn" id="prevDevice">◀</button>
          <button class="carousel-btn" id="nextDevice">▶</button>
        </div>
      `
    },
    {
      title: "Firmware Installation",
      description: "Verbinde dein Gerät per USB und starte den Flash-Vorgang.",
      content: () => {
        const device = devices[selectedDeviceIndex];
        return `
          <div class="flash-card">
            <div class="flash-icon">⚡</div>
            <h3>${device.name}</h3>
            <p>Firmware-Version: <strong>v${device.version}</strong></p>
            <button id="myFlashBtn" class="flashBtn">Jetzt Installieren</button>
            <esp-web-install-button id="espButton" style="display:none;"></esp-web-install-button>
            <div class="flash-console hidden" id="flashConsole">
              <esp-web-install-console></esp-web-install-console>
            </div>
          </div>
        `;
      }
    },
    {
      title: "WLAN-Einrichtung starten",
      description: "Scanne den QR-Code mit deinem Smartphone, um dich mit dem Konfigurations-WLAN des Geräts zu verbinden. Danach startet automatisch das Einrichtungsportal für die WLAN-Einstellungen.",
      content: () => `<div class="qrcode"><canvas id="qrcode"></canvas></div>`
    },
    {
      title: "Geräte-IP abrufen",
      description: "Ermittle die lokale IP deines ESP32, um direkt auf das Webinterface zuzugreifen.",
      content: () => `
        <div class="ip-card">
          <div class="ip-icon">🚀</div>
          <h3 id="ipTitle">IP-Adresse ermitteln</h3>
          <p id="deviceIp">Klicke unten, um die IP zu ermitteln.</p>
          <button id="getIpBtn" class="flashBtn">IP abrufen</button>
          <div id="ipActions" class="ip-actions hidden">
            <div class="ip-subcard browser">
              <h4>Im Browser öffnen</h4>
              <button id="openInBrowser" class="ipBtn">Öffnen</button>
            </div>
            <div class="ip-subcard qr">
              <h4>QR-Code für Smartphone</h4>
              <div id="ipQrcode"></div>
            </div>
          </div>
        </div>
      `
    },
    {
      title: "",
      description: "",
      content: () => `
        <div class="success-step">
          <div class="success-emoji">🎉</div>
          <h2>Geschafft!</h2>
          <p class="step-description">Dein Gerät ist bereit und einsatzbereit. Viel Spaß beim Testen!</p>
        </div>
      `
    }
  ];

  function renderStep() {
    const step = steps[currentStep];
    let html = "";
    if (step.title) html += `<h2>${step.title}</h2>`;
    if (step.description) html += `<p class="step-description">${step.description}</p>`;
    html += step.content();
    stepContent.innerHTML = html;

    if (currentStep === 0) {
      backButton.style.display = "none";
      nextButton.style.display = "none";
      pageIndicator.style.display = "none";
      document.getElementById("startAssistantBtn").addEventListener("click", () => {
        currentStep = 1;
        renderStep();
      });
    }

    if (currentStep === 1) {
      nextButton.style.display = "inline-block";
      backButton.style.display = "none";
      pageIndicator.style.display = "none";
      renderCarousel();
    }

    if (currentStep === 2) {
      const device = devices[selectedDeviceIndex];
      const espButton = document.getElementById('espButton');
      espButton.setAttribute("manifest", `assets/devices/${device.folder}/firmware/manifest.json`);
      const myFlashBtn = document.getElementById('myFlashBtn');
      myFlashBtn.addEventListener('click', async () => {
        const shadowBtn = await waitForShadowButton(espButton);
        if (shadowBtn) shadowBtn.click();
      });
    }

    if (currentStep === 3) {
      const device = devices[selectedDeviceIndex];
      const qrCanvas = document.getElementById("qrcode");
      const wifiString = `WIFI:T:nopass;S:${device.ap_ssid};;`;
      QRCode.toCanvas(qrCanvas, wifiString, { width: 200 }, function (error) {
        if (error) console.error(error);
      });
      const ssidText = document.createElement("p");
      ssidText.innerHTML = `${device.ap_ssid}`;
      ssidText.style.marginTop = "2px";
      ssidText.style.fontSize = "12px";
      ssidText.style.color = "#374151";
      ssidText.style.fontWeight = "600";
      ssidText.style.textAlign = "center";
      ssidText.style.fontFamily = "monospace";
      qrCanvas.insertAdjacentElement("afterend", ssidText);
    }

    if (currentStep === 4) {
      const getIpBtn = document.getElementById("getIpBtn");
      const ipTitle = document.getElementById("ipTitle");
      const deviceIp = document.getElementById("deviceIp");
      const ipActions = document.getElementById("ipActions");
      const openInBrowserBtn = document.getElementById("openInBrowser");
      const ipQrcodeContainer = document.getElementById("ipQrcode");

      ipTitle.textContent = "IP-Adresse ermitteln";
      deviceIp.style.display = "block";
      deviceIp.textContent = "Klicke unten, um die IP zu ermitteln.";
      ipActions.classList.add("hidden");
      ipActions.classList.remove("show");
      ipQrcodeContainer.innerHTML = "";
      openInBrowserBtn.onclick = null;

      const freshBtn = getIpBtn.cloneNode(true);
      getIpBtn.parentNode.replaceChild(freshBtn, getIpBtn);

      freshBtn.addEventListener("click", async () => {
        freshBtn.disabled = true;
        deviceIp.textContent = "Verbinde mit ESP32...";
        try {
          const ip = await getEspIp(6000);
          if (ip) {
            ipTitle.textContent = `Gefundene IP: ${ip}`;
            deviceIp.style.display = "none";
            freshBtn.style.display = "none";
            ipActions.classList.remove("hidden");
            ipActions.classList.add("show");

            openInBrowserBtn.onclick = () => window.open(`http://${ip}`, "_blank");

            const canvas = document.createElement("canvas");
            ipQrcodeContainer.appendChild(canvas);
            QRCode.toCanvas(canvas, `http://${ip}`, { width: 200 }, (error) => {
              if (error) console.error(error);
            });
          } else {
            deviceIp.textContent = "Keine IP erhalten!";
            freshBtn.disabled = false;
          }
        } catch (err) {
          deviceIp.textContent = "Fehler: " + err.message;
          freshBtn.disabled = false;
        }
      });
    }

    if (currentStep === totalSteps - 1) {
      const confettiContainer = document.createElement("div");
      confettiContainer.className = "confetti";
      stepContent.appendChild(confettiContainer);
      for (let i = 0; i < 100; i++) {
        const piece = document.createElement("div");
        piece.className = "confetti-piece";
        piece.style.left = Math.random() * 100 + "%";
        piece.style.animationDuration = 2 + Math.random() * 3 + "s";
        piece.style.backgroundColor = `hsl(${Math.random()*360}, 100%, 60%)`;
        confettiContainer.appendChild(piece);
      }
      const restartBtn = document.createElement("button");
      restartBtn.className = "restartBtn";
      restartBtn.textContent = "Erneut ausführen";
      restartBtn.style.display = "block";
      restartBtn.style.margin = "0px auto 0 auto";
      restartBtn.addEventListener("click", () => {
        currentStep = 0;
        pageIndicator.innerHTML = "";
        renderStep();
      });
      stepContent.appendChild(restartBtn);
    } else {
      stepContent.style.display = "block";
    }

    updateButtons();

    if (currentStep >= 2 && currentStep < totalSteps - 1) {
      renderIndicator();
      pageIndicator.style.display = "flex";
    } else {
      pageIndicator.style.display = "none";
      pageIndicator.innerHTML = "";
    }
  }

  function renderCarousel() {
    const wrapper = document.getElementById("carouselWrapper");
    const indicator = document.getElementById("selectorIndicator");

    function updateView() {
      wrapper.innerHTML = "";
      const visible = devices.slice(startIndex, startIndex + 3);
      visible.forEach((d, i) => {
        const idx = startIndex + i;
        const card = document.createElement("div");
        card.className = "device-card" + (idx === selectedDeviceIndex ? " selected" : "");
        card.dataset.index = idx;
        card.innerHTML = `
          <span class="version">v${d.version}</span>
          <div class="image-box"><img src="assets/devices/${d.folder}/product.png" alt="${d.name}"></div>
          <h3>${d.name}</h3>
        `;
        card.addEventListener("click", () => {
          selectedDeviceIndex = idx;
          updateView();
        });
        wrapper.appendChild(card);
      });
      if (indicator) {
        indicator.innerHTML = "";
        devices.forEach((_, i) => {
          const dot = document.createElement("div");
          dot.className = "dot" + (i === selectedDeviceIndex ? " active" : "");
          indicator.appendChild(dot);
        });
      }
      document.getElementById("prevDevice").disabled = startIndex === 0;
      document.getElementById("nextDevice").disabled = startIndex >= devices.length - 3;
    }

    document.getElementById("prevDevice").onclick = () => { if (startIndex > 0) startIndex--; updateView(); };
    document.getElementById("nextDevice").onclick = () => { if (startIndex < devices.length - 3) startIndex++; updateView(); };
    updateView();
  }

  function waitForShadowButton(el) {
    return new Promise(resolve => {
      const interval = setInterval(() => {
        const btn = el.shadowRoot?.querySelector('button');
        if (btn) { clearInterval(interval); resolve(btn); }
      }, 100);
    });
  }
  let espPort = null;
  async function getEspIp(timeoutMs = 6000) {
    let reader, writer;
    let writerPipe, readerPipe;
    let port;

    try {
      if (!espPort) {
        const granted = await navigator.serial.getPorts();
        espPort = granted[0] || await navigator.serial.requestPort();
      }
      port = espPort;

      if (!port.readable || !port.writable) {
        await port.open({ baudRate: 115200 });
      }

      const encoder = new TextEncoderStream();
      writerPipe = encoder.readable.pipeTo(port.writable);
      writer = encoder.writable.getWriter();

      const decoder = new TextDecoderStream();
      readerPipe = port.readable.pipeTo(decoder.writable);
      reader = decoder.readable.getReader();

      await writer.write("GET_IP\n");

      const readLoop = (async () => {
        let buffer = "";
        while (true) {
          const { value, done } = await reader.read();
          if (done) break;
          if (value) {
            // collapse newlines and search for any IP in the stream
            buffer += String(value).replace(/[\r\n]+/g, "");
            const m = buffer.match(/\b\d{1,3}(?:\.\d{1,3}){3}\b/);
            if (m) return m[0];
          }
        }
        return null;
      })();

      const timeout = new Promise((_, reject) => {
        const t = setTimeout(() => {
          clearTimeout(t);
          reject(new Error("Timeout – keine Antwort vom ESP32"));
        }, timeoutMs);
      });

      const ip = await Promise.race([readLoop, timeout]);
      return ip;
    } catch (err) {
      console.error("Fehler bei WebSerial:", err);
      throw err;
    } finally {
      try { await reader?.cancel(); } catch {}
      try { reader?.releaseLock(); } catch {}
      try { await writer?.close(); } catch {}
      try { writer?.releaseLock(); } catch {}
      try { await readerPipe?.catch(()=>{}); } catch {}
      try { await writerPipe?.catch(()=>{}); } catch {}
      try { await port?.close(); } catch {}
      espPort = null; // force fresh open next time (prevents stuck state)
    }
  }

  function updateButtons() {
    if (currentStep === 0) {
      backButton.style.display = "none";
      nextButton.style.display = "none";
      pageIndicator.style.display = "none";
      return;
    }
    if (currentStep === totalSteps - 1) {
      backButton.style.display = "none";
      nextButton.style.display = "none";
      pageIndicator.style.display = "none";
      return;
    }
    if (currentStep === 1) {
      backButton.style.display = "none";
      pageIndicator.style.display = "none";
      nextButton.style.display = "inline-block";
      nextButton.textContent = "Next →";
      nextButton.style.marginLeft = "auto";
      return;
    } else {
      nextButton.style.marginLeft = "";
    }
    backButton.style.display = "inline-block";
    nextButton.style.display = "inline-block";
    nextButton.textContent = (currentStep === totalSteps - 2) ? "Fertigstellen" : "Next →";
    pageIndicator.style.display = "flex";
  }

  function renderIndicator() {
    pageIndicator.innerHTML = "";
    for (let i = 1; i < totalSteps; i++) {
      const dot = document.createElement("div");
      dot.className = "dot" + (i === currentStep ? " active" : "");
      pageIndicator.appendChild(dot);
    }
  }

  backButton.addEventListener("click", () => {
    if (currentStep > 1) {
      currentStep--;
      renderStep();
    } else if (currentStep === 1) {
      currentStep = 0;
      renderStep();
    }
  });

  nextButton.addEventListener("click", () => {
    if (currentStep < totalSteps - 1) {
      currentStep++;
      renderStep();
    }
  });

  renderStep();
});
