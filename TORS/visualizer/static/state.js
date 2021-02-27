var svg_layout;

$(document).ready(function () {
    load_layout(refresh_state);
	setup_collapsables()
});

function load_layout(cb) {
    /*
     * Load the layout and cache the image for later use
     */
    Snap.load("/engine/location", svg => {
            svg_layout = svg;
            cb();
        }
    );
}

function refresh_state() {
    /*
     * Load a new state
     */
    $.getJSON("/engine/state", function (data) {
        update_info(data);
        update_layout(data);
    });
}

function update_info(data) {
    /*
     * Update state info
     * :param data: the json object with information about the state
     * :type: string
     */
    $("#current-time").html(data.time);
    $("#next-event").html(data.next_event);

    var shunting_units = "";
    $.each(data.tracks, function(i, trains){
        $.each(trains, function(i, train){
            var color = get_train_color(train.id);
			var title = '';
			$.each(train.train_units, function(k, unit){
				title += (k > 0) ? "\n" : '';
				title += unit + ' (' + train.train_unit_types[k] + ') ' + train.train_unit_tasks[k] ;
			});
            shunting_units += "<p style='color: " + color + ";' title='"+title+"'>" + train.id + " [" + train.train_units + "]</p>"
        });
    });
    $("#shunting-units").html(shunting_units);
	
	var timetable = $("#time-table")[0];
	$("#time-table tr").remove();
    add_timetable_header(timetable);
	
	$.each(data.goals, function (key, val) {
		var row = timetable.insertRow(-1);
		row.setAttribute("class", "goal-item");
		var time = row.insertCell(0);
		var type = row.insertCell(1);
		var tracks = row.insertCell(2);
		var train = row.insertCell(3);

		// Set content for each cell
		time.appendChild(document.createTextNode(val.time));
		type.appendChild(document.createTextNode(val.type));
		tracks.appendChild(document.createTextNode(val.from + "->" + val.to));
		var traindescription = val.id + " \t[ ";
		if(val.train_units.toString().replace(",", "") > 0) traindescription += val.train_units + " | ";
		traindescription += val.train_unit_types + " ]";
		train.appendChild(document.createTextNode(traindescription));

		
	});
}

function update_layout(data) {
    /*
     * Update the layout (trains and reserved tracks)
     * :param data: the json object with information about the state
     * :type: string
     */

    // clear
    layout = Snap('#layout');

    var svg_width = svg_layout.node.getAttribute("width");
    var svg_height = svg_layout.node.getAttribute("height");

    layout.attr({
        width: svg_width,
        height: svg_height
    });

    layout.clear();
    layout.append(Snap(svg_layout.node.cloneNode(true)));

    // display trains on tracks
    $.each(data.tracks, function (i, trains) {
		
		
		// if there are trains on the track
        if (trains.length) {
			var track_line = document.getElementById("track-" + i);
            var segment = track_line.getTotalLength() / trains.length;
            var end = 0;
            var sample_interval = 5;
			
			var totalLength = 0;
			$.each(trains, function (i, train) {
				totalLength += train.length;
			});
			var scale = 1.0
			if (totalLength > track_line.getTotalLength()) {
				scale = track_line.getTotalLength() / totalLength;
			} else {
				end = Math.floor((track_line.getTotalLength() - totalLength) / 2);
			}
			
        }
		

        // display trains
        $.each(trains, function (i, train) {
            var start = end;
            var points = "M";
			var segment = train.length * scale
            // collect points on track
            for (var t = start; t <= start + train.length * scale + sample_interval; t += sample_interval) {
                point = track_line.getPointAtLength(t);
                points += point.x + " " + point.y + " ";
                end = t;
            }

            // draw train
            var train_line = layout.path(d = points);

            var color = get_train_color(train.id);

            train_line.attr({
                id: "train-" + train.unique_id,
                strokeWidth: 14,
                stroke: color,
                fill: "none"
            });

            // display direction
            if (train.direction && !train.in_neutral && !train.moving) {

                var point1 = train_line.getPointAtLength(10);
                var point2 = train_line.getPointAtLength(train_line.getTotalLength() - 10);

                // calculate which end of the train is closest to direction point
                var a = point1.x - train.direction[0];
                var b = point1.y - train.direction[1];
                var c = Math.sqrt(a * a + b * b);

                var a2 = point2.x - train.direction[0];
                var b2 = point2.y - train.direction[1];
                var c2 = Math.sqrt(a2 * a2 + b2 * b2);

                //draw circle
                var center = (c < c2) ? point1 : point2;
                var direction = layout.circle(center.x, center.y, 4);

                direction.attr({
                    id: "direction-" + train.unique_id,
                    fill: "#f0f0f0"
                });
            }
        });
    });

    // display reserved tracks
    $.each(data.reserved_tracks, function (i, track) {
        var reserved_track = $("#track-" + track);
        if (reserved_track.length) {
            reserved_track.attr({
                stroke: "#a12221"
            });
        }
    });
}

function get_train_color(train) {
    // get color for train
    var x = train;
    x = x ^ x << 13;
    x = x ^ x >> 17;
    x = x ^ x << 5;
    x = x % 16777215;
    return '#' + (0x1000000 + x * 0xffffff).toString(16).substr(1, 6);
}

function add_timetable_header(table) {
	table.innerHTML =
        "<tr>\n" +
        "<th style='min-width: 60px'><i class='material-icons'>access_time</i> Time</th>\n" +
        "<th style='min-width: 70px'><i class='material-icons'>touch_app</i> Type</th>\n" +
        "<th style='min-width: 110px'><i class='material-icons'>directions</i> Tracks</th>\n" +
        "<th style='min-width: 180px'><i class='material-icons'>train</i> Train</th>\n" +
        "</tr>";
}

function setup_collapsables() {
	var coll = document.getElementsByClassName("collapsible");
	var i;

	for (i = 0; i < coll.length; i++) {
	  coll[i].addEventListener("click", function() {
		this.classList.toggle("active");
		var content = this.nextElementSibling;
		if (content.style.display === "block") {
		  content.style.display = "none";
		} else {
		  content.style.display = "block";
		}
	  });
	}

}
