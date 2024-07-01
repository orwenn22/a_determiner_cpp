///This function can be used to download a file from emscripten's filesystem
function downloadFromFS(path) {
    path = path.toString();     //No idea if this is useful or not

    //Navigating to the correct path/file
    const pathParts = path.split('/').filter(part => part);
    let currentDir = FS.root;
    for (let i = 0; i < pathParts.length; i++) {
        if (!currentDir.contents[pathParts[i]]) {
            throw new Error("File or directory not found");
        }
        currentDir = currentDir.contents[pathParts[i]];
    }

    //Make sure the content exist
    //TODO : ensure the thing is a file (not a directory)? Maybe using "mode" ?
    if (!currentDir.contents || typeof currentDir.contents !== 'object') {
        throw new Error("Not a file");
    }

    //Prepare the content for download
    const fileContents = currentDir.contents;
    const uint8Array = new Uint8Array(fileContents);
    const blob = new Blob([uint8Array], { type: 'application/octet-stream' });

    //Dirty trick to make it download the file
    const link = document.createElement('a');
    link.href = URL.createObjectURL(blob);
    link.download = pathParts[pathParts.length - 1];  // Use the file name from the path
    link.click();
    URL.revokeObjectURL(link.href);
}
