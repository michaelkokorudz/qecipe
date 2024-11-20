function apiCall(url, data = null, callback = null) {
    if (!url.startsWith("http://") && !url.startsWith("https://")) url = "http://localhost:8000/" + url;
    $.ajax({
        method: "GET",
        url: url,
        data: data
    })
        .done(function (msg) {
            if (callback != null) {
                callback.call(this, msg);
            }
        })
        .fail(function (jqXHR, textStatus) {
            alert("Request failed: " + textStatus);
        })
}