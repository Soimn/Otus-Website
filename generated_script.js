var DevLogIDs = ["maed200614"];

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
			if (log_objects[i].className.includes("log_title_minimized") ||
			    log_objects[i].className.includes("log_title_maximized"))
			{
				log_objects[i].className = log_id.concat(" log_title_".concat(state_string));
			}

			else if (log_objects[i].className.includes("log_date_minimized") ||
				 log_objects[i].className.includes("log_date_maximized"))
			{
				log_objects[i].className = log_id.concat(" log_date_".concat(state_string));
			}

			else if (log_objects[i].className.includes("log_content_minimized") ||
				 log_objects[i].className.includes("log_content_maximized"))
			{
				log_objects[i].className = log_id.concat(" log_content_".concat(state_string));
			}

			else
			{
				console.log("The dev log (id: {0}) is missing the log_* class", log_id);
			}
		}
	}
}

function SwitchTab()
{
	var current_tab = window.location.hash;

	var about = document.getElementById("about_tab");
	var docs  = document.getElementById("docs_tab");
	var log   = document.getElementById("log_tab");

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

		else
		{
			SetLogState(maximized_log, false);
			maximized_log = null;
		}
	}

	else
	{
		window.location.hash = "#about";
		about.style.display  = "block";
		docs.style.display   = "none";
		log.style.display    = "none";
	}

	if (current_tab.startsWith("#log"))
	{
		if (maximized_log != null)
		{
			for (title of document.getElementsByClassName("log_title_minimized")) title.style.display = "none";
			for (date  of document.getElementsByClassName("log_date_minimized"))  date.style.display  = "none";
		}

		else
		{
			for (title of document.getElementsByClassName("log_title_minimized")) title.style.display = "block";
			for (date  of document.getElementsByClassName("log_date_minimized"))  date.style.display  = "block";
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

		var log_tab = document.getElementById("log_tab");

		var log_entries = [];
		for (child of log_tab.children) log_entries.push(child);

		log_entries.sort(function (a, b) {
			var a_date = a.className.toString().replace(/[^0-9]/g, "");
			var b_date = b.className.toString().replace(/[^0-9]/g, "");

			return (a_date > b_date ? -1 : (b_date > a_date ? 1 : 0));
		});

		for (entry of log_entries) log_tab.appendChild(entry);

		SwitchTab();
	}
});

window.addEventListener("hashchange", SwitchTab);
