document.getElementById('alertBtn').addEventListener('click', () => {
    alert('Hallo! Du hast die PWA getestet 🎉');
});
let deferredPrompt;

window.addEventListener('beforeinstallprompt', (e) => {
    // Verhindere, dass Chrome sofort die Banner-Aufforderung zeigt
    e.preventDefault();
    deferredPrompt = e;

    // Zeige deinen eigenen Install-Button
    const installBtn = document.createElement('button');
    installBtn.id = 'installBtn';
    installBtn.innerText = 'App installieren';
    installBtn.style.padding = '1rem 2rem';
    installBtn.style.fontSize = '1rem';
    installBtn.style.marginTop = '20px';
    document.body.appendChild(installBtn);

    installBtn.addEventListener('click', async () => {
        // Zeige das Installations-Prompt
        deferredPrompt.prompt();
        const { outcome } = await deferredPrompt.userChoice;
        if (outcome === 'accepted') {
            console.log('PWA Installation akzeptiert ✅');
        } else {
            console.log('PWA Installation abgelehnt ❌');
        }
        deferredPrompt = null;
        installBtn.remove(); // Button entfernen, wenn installiert/abgelehnt
    });
});