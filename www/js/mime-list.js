function fillMimeSelect() {
    var mime_select = document.getElementById('mime-select');
    var mimes = CodeMirror.listMIMEs()
    for (var i in mimes) {
        var opt = document.createElement("option");
        opt.value = mimes[i].mime;
        opt.text = mimes[i].mime;
        mime_select.appendChild(opt);
    }
}

fillMimeSelect();

function onMimeChanged() {
    var mime_select = document.getElementById('mime-select');
    myCodeMirror.setOption("mode", mime_select.value);
}
