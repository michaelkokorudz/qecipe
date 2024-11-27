function apiCall(url, data = null, callback = null) {
    console.log("API Call: " + url);
    url = "http://127.0.0.1:5000/" + url;

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
