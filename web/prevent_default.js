//The level editor use the ctrl+s shortcut. However, on most web browser, hitting that key combo will trigger the
//"save page" popup. This code prevent that from happening.
document.addEventListener("keydown", function(e) {
    if(e.key !== "F12" && e.key !== "F11") {
        e.preventDefault();
    }
});
