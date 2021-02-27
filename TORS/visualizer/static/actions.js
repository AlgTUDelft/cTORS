$(document).ready(function () {
    refresh_actions();
});

function refresh_actions() {
    /*
     * Loads and displays all available actions
     */
    var table = document.getElementById("action-table");
    $("#action-table tr").remove();
    add_table_header(table);

    $.getJSON("/engine/actions", function (data) {

        items = [];

        $.each(data, function (key, val) {
            var row = table.insertRow(-1);
            row.setAttribute("class", "action-item");
            row.setAttribute("id", key);
            row.setAttribute("onclick", "perform_action(" + key + ")");
            row.setAttribute("onmouseover", "highlight_action(" + JSON.stringify(val.train_ids) + "," + JSON.stringify(val.tracks) + ")");
            row.setAttribute("onmouseout", "dehighlight_action(" + JSON.stringify(val.train_ids) + "," + JSON.stringify(val.tracks) + ")");
            var action_id = row.insertCell(0);
            var action = row.insertCell(1);
            var trains = row.insertCell(2);
            var tracks = row.insertCell(3);
            //var employees = row.insertCell(4);
            var duration = row.insertCell(4);
            //var reward = row.insertCell(6);

            // Set content for each cell
            action_id.appendChild(document.createTextNode(key));
            action.appendChild(document.createTextNode(get_action(key, val)));
            trains.appendChild(document.createTextNode(get_trains(val.trains)));
            tracks.appendChild(document.createTextNode(get_tracks(val.track_names)));
            //employees.appendChild(document.createTextNode(val.employees));
            duration.appendChild(document.createTextNode(val.duration));
            //reward.appendChild(document.createTextNode(val.reward));
        });

    });
}

function perform_action(id) {
    /*
     * Performs an action and refreshes the new actions and state
     * :param id: the id of the action that is performed
     * :type: int
     */
    $.ajax({
        url: '/engine/actions?action=' + id,
        type: 'PUT',
        success: function (result) {
            refresh_actions();
            refresh_state();
        }
    });
}

function add_table_header(table) {
    table.innerHTML =
        "<tr>\n" +
        "<th style='min-width: 20px'>Id</th>\n" +
        "<th style='min-width: 150px'><i class='material-icons'>touch_app</i> Action</th>\n" +
        "<th style='min-width: 70px'><i class='material-icons'>train</i> Trains</th>\n" +
        "<th style='min-width: 70px'><i class='material-icons'>directions</i> Tracks</th>\n" +
        //"<th style='min-width: 110px'><i class='material-icons'>person</i> Employees</th>\n" +
        "<th style='min-width: 100px'><i class='material-icons'>watch_later</i> Duration</th>\n" +
        //"<th style='min-width: 90px'><i class='material-icons'>star</i> Reward</th>\n" +
        "</tr>"
}

function get_action(key, action) {
    var content = action.type;

    if ("task" in action) {
        content += " - " + action.task
    }

    return content;
}

function get_tracks(tracks) {
    if (tracks.length > 5)
        return tracks.slice(0, 2).join(", ") + " ... " + tracks.slice(tracks.length - 3, tracks.length).join(", ");
    return tracks.join(", ");
}

function get_trains(trains) {
    /*
     * Takes the first part of the train UUID's and makes a list
     * :param key: trains to be displayed
     * :type: List[string]
     *
     * :returns: string containing all trains with shortened id
     * :rtype: string
     */
    var result = '';
    $.each(trains, function (i, train) {
        result += (i > 0) ? ", " : '';
        result += train['id'] + '[' + train['train_units'] + ']' //train.split("-")[0];
    });

    return result;
}

function highlight_action(trains, tracks) {
    /*
     * Highlights the shunting units (and tracks) on which the action will be performed
     * :param trains: list of trains to be highlighted
     * :type: List[string]
     * :param tracks: tracks to be highlighted
     * :type: List[string]
     */
    $.each(trains, function (i, train) {
        set_obj_color("#train-" + train, 50);
    });
	var facilities = $("[id^=facility-]");
    $.each(tracks, function (i, track) {
        set_obj_color("#track-" + track, 120);
		$.each(facilities, function (j, facility) {
			if(facility.id.endsWith("-"+track)) {
				set_obj_color(facility, 50);
			}
		});
    });
}

function dehighlight_action(trains, tracks) {
    /*
     * Turns off highlighting of shunting units (and track)
     * :param trains: list of trains to turn off highlighting
     * :type: List[string]
     * :param tracks: tracks to turn off highlighting
     * :type: List[string]
     */
    $.each(trains, function (i, train) {
        restore_obj_color("#train-" + train);
    });
	var facilities = $("[id^=facility-]");
    $.each(tracks, function (i, track) {
        restore_obj_color("#track-" + track);
		$.each(facilities, function (j, facility) {
			if(facility.id.endsWith(track)) {
				restore_obj_color(facility);
			}
		});
    });
}

function set_obj_color(id, amount) {
    /*
     * Sets the color for an object
     * :param id: id of object to be colored
     * :type: string
     * :param amount: amount to lighten/darken color
     * :type: int
     */
	var obj = $(id);
    if (obj.length && !obj.attr('org_color')) {
        var org_color = obj.attr('stroke');
        var color = change_color(org_color, amount);
        obj.attr({
            stroke: color,
            org_color: org_color
        });
    }
}

function restore_obj_color(id) {
    /*
     * Restores the color for an object
     * :param id: id of object to be restored
     * :type: string
     */
    var obj = $(id);
    if (obj.length && obj.attr('org_color')) {
        var color = obj.attr('org_color');
        obj.attr({
            stroke: color
        });
		obj.removeAttr("org_color")
    }
}

function change_color(color, amount) {
    /*
     * Makes the color lighter/darker by amount
     * :param color: color that needs to be darkened
     * :type: string
     * :param amount: amount of lightening/darkening
     * :type: int
     *
     * :returns: lightened/darkened color
     * :rtype: string
     */

    var R = parseInt(color.substring(1, 3), 16);
    var G = parseInt(color.substring(3, 5), 16);
    var B = parseInt(color.substring(5, 7), 16);

    R = R + amount;
    G = G + amount;
    B = B + amount;

    R = (R < 255) ? R : 255;
    R = (R > 0) ? R : 0;

    G = (G < 255) ? G : 255;
    G = (G > 0) ? G : 0;

    B = (B < 255) ? B : 255;
    B = (B > 0) ? B : 0;

    var RR = ((R.toString(16).length == 1) ? "0" + R.toString(16) : R.toString(16));
    var GG = ((G.toString(16).length == 1) ? "0" + G.toString(16) : G.toString(16));
    var BB = ((B.toString(16).length == 1) ? "0" + B.toString(16) : B.toString(16));

    return "#" + RR + GG + BB;

}
