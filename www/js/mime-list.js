function setCookie(name, value)
{
    var exdate = new Date();
    exdate.setDate(exdate.getDate() + 365 * 42);
    document.cookie = name + "=" + escape(value) + "; expires=" + exdate.toUTCString();
}

function getCookie(name)
{
    var i, x, y, ARRcookies = document.cookie.split(";");
    for (i = 0; i < ARRcookies.length; i++) {
        x = ARRcookies[i].substr(0, ARRcookies[i].indexOf("="));
        y = ARRcookies[i].substr(ARRcookies[i].indexOf("=") + 1);
        x = x.replace(/^\s+|\s+$/g, "");
        if (x == name)
            return unescape(y);
    }
}

function fillMimeSelect() {
    var mime_select = document.getElementById('mime-select');
    var mimes = CodeMirror.mimeModes;
    for (var i in mimes) {
        var opt = document.createElement("option");
        opt.value = i;
        opt.text = i;
        mime_select.appendChild(opt);
    }
}

fillMimeSelect();

function onMimeChanged() {
    var mime_select = document.getElementById('mime-select');
    myCodeMirror.setOption("mode", mime_select.value);
}

function selectTheme() {
    var theme_select = document.getElementById('theme-select');
    myCodeMirror.setOption("theme", theme_select.value);
    setCookie("theme", theme_select.value);
}

var theme = getCookie("theme");
if (theme != null && theme != '') {
    myCodeMirror.setOption("theme", theme);
    if (document.getElementById('theme-select'))
        document.getElementById('theme-select').value = theme;
}

if (document.getElementById('theme-select'))
    document.getElementById('theme-select').onchange = selectTheme;
