var DevLogIDs = ["t200610"];

var maximized_log = null;

function SetLogState(log_id, should_maximize)
{
    var state_string = (should_maximize ? "maximized" : "minimized");

    var log_objects = document.getElementsByClassName(log_id)
    if (log_objects === undefined || log_objects.length === 0)
    {
        console.log("The dev log (id: {0}) is missing the title, date and content objects", log_id);
    }

    else
    {
        for (var i = 0; i < log_objects.length; ++i)
        {
            if (log_objects[i].className.includes("log_content_minimized") ||
                log_objects[i].className.includes("log_content_maximized"))
            {
                log_objects[i].className = log_id.concat(" log_content_".concat(state_string));
            }

            else if (!(log_objects[i].className.includes("log_title") ||
                       log_objects[i].className.includes("log_date")))
            {
                console.log("The dev log (id: {0}) is missing the log_*_* class", log_id);
            }
        }
    }
}

function SwitchTab()
{
	var current_tab = window.location.hash;

	about = document.getElementById("about_tab");
	docs  = document.getElementById("docs_tab");
	log   = document.getElementById("log_tab");

	if (current_tab === "#docs")
	{
		about.style.display = "none";
		docs.style.display  = "block";
		log.style.display   = "none";
	}

	else if (current_tab.startsWith("#log"))
	{
		about.style.display = "none";
		docs.style.display  = "none";
		log.style.display   = "block";
	}

	else
	{
		window.location.hash = "#about";
		about.style.display  = "block";
		docs.style.display   = "none";
		log.style.display    = "none";
	}

	if (current_tab.startsWith("#log@"))
	{
		var log_id = current_tab.slice("#log@".length);

        if (DevLogIDs.find(x => x === log_id) === undefined)
        {
            window.location.hash = "#log";

            if (maximized_log != null)
            {
                SetLogState(maximized_log, false);
                maximized_log = null;
            }
        }

        else
        {
            if (maximized_log != null)
            {
                SetLogState(maximized_log, false);
            }

            maximized_log = log_id;
            SetLogState(log_id, true);
        }
	}

    else if (maximized_log != null)
    {
        SetLogState(maximized_log, false);
        maximized_log = null;
    }

    window.scrollTo(0, 0);
}

document.addEventListener("readystatechange", event => {
	if (event.target.readyState === "complete")
	{
        if (navigator.userAgent.match(/(iphone)|(ipad)/i))
        {
            var view = document.querySelector("meta[name=\"viewport\"]");

            if (view)
            {
                view.content = "width=device-width, minimum-scale=1.0, maximum-scale=1.0, initial-scale=1.0";
                window.addEventListener("gesturestart", function() {});
            }
        }

		SwitchTab();
	}
});

window.addEventListener("hashchange", SwitchTab);