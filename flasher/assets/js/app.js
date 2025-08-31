document.addEventListener('DOMContentLoaded', () => {
  const espButton = document.getElementById('espButton');
  const myFlashBtn = document.getElementById('myFlashBtn');

  myFlashBtn.addEventListener('click', async () => {
    const shadowBtn = await waitForShadowButton(espButton);
    if (shadowBtn) {
      shadowBtn.click();
      console.log('⚡ Flash gestartet');
    } else {
      console.error('Konnte den inneren ESP Button nicht finden!');
    }
  });

  function waitForShadowButton(el) {
    return new Promise((resolve) => {
      const interval = setInterval(() => {
        const btn = el.shadowRoot?.querySelector('button');
        if (btn) {
          clearInterval(interval);
          resolve(btn);
        }
      }, 100);
    });
  }
});
