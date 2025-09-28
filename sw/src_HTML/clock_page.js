/* clock_page.js
   Justin Thai
   Walter Contreras
   09/24/2025
*/


var client      = null;
var hour        = "";
var minute      = "";
var second      = "";
var ampm        = "";
var mil_time    = "";

/* broker info */
var hostname        = "broker.emqx.io:8083/mqtt";
var port            = "8083";

var eid             = "jtt2525"
var clientId        = "justin";

// Subscribe (from board_2_webpage)
var test 	= 	eid + "/test";
var hour_bd =   eid + "/b2w/hour"; 
var min_bd  =   eid + "/b2w/min";
var sec_bd  =   eid + "/b2w/sec"; 
var mode_bd =   eid + "/b2w/mode"   
var am_or_pm =  eid + "/b2w/am_or_pm"

// Publish (from webpage_2_board) 
var w2b   =  eid + "/w2b";


// -----------------------------------------------------------------------
// This is called after the webpage is completely loaded
// It is the main entry point into the JS code
function connect() {
	// Set up the client
	const url = 'ws://broker.emqx.io:8083/mqtt';

	const options = {
		// Clean session
		clean: true,
		connectTimeout: 4000,
		// Authentication
		clientId: eid,
		username: null,
		password: null,
	};
	client  = mqtt.connect(url, options);
	client.on('connect', function () {
		onConnect();
	});

	// Receive messages
	client.on('message', function (topic, payload, packet) {
	  	onMessageArrived(topic, payload);
	});
}


function onConnect() {
	console.log("Client Connected.");
    
	// TODO: subscribe to your desired topics.
	client.subscribe(test);
	client.subscribe(hour_bd);
	client.subscribe(min_bd);
	client.subscribe(sec_bd);
    client.subscribe(mode_bd);
	client.subscribe(am_or_pm);
}



// -----------------------------------------------------------------------
// Provides the button logic that toggles the mode
// Triggered by pressing the HTML button "12/24"
//
function _12_24_onClick() {
	var payload = "12/24";
	console.log("Publishing ", payload, " to ", w2b);
	client.publish(w2b, payload);
}


/* toggles darkmode; on click for dark mode button*/
function darkMode_onClick() {
	var payload = "toggle darkmode";
	console.log("Publishing ", payload, " to ", w2b);
	client.publish(w2b, payload);
}


/* dance; on click for dance mode button*/
function dance_onClick() {
	var payload = "dance";
	console.log("Publishing ", payload, " to ", w2b);
	client.publish(w2b, payload);
}


/* toggle AM/PM; on click for AM/PM button*/
function toggleAM_PM(){
	var payload = "am or pm"
	console.log("Publishing ", payload, " to ", w2b);
	client.publish(w2b, payload);
}


/* decrement time hours; on click for hour-- button*/
function dec_hour() {
	var payload = "hour--";
	console.log("Publishing ", payload, " to ", w2b);
	client.publish(w2b, payload);
}


/* increment time hours; called on click for hour++ button*/
function inc_hour(){
	var payload = "hour++";
	console.log("Publishing ", payload, " to ", w2b);
	client.publish(w2b, payload);
}


/* decrement time min; called on click for min-- button*/
function dec_min(){
	var payload = "minute--";
	console.log("Publishing ", payload, " to ", w2b);
	client.publish(w2b, payload);
}


/* increment time min; called on click for min++ button*/
function inc_min(){
	var payload = "minute++";
	console.log("Publishing ", payload, " to ", w2b);
	client.publish(w2b, payload);
}


/* decrement time sec; called on click for sec-- button*/
function dec_sec(){
	var payload = "second--";
	console.log("Publishing ", payload, " to ", w2b);
	client.publish(w2b, payload);
}


/* increment time sec; called on click for sec++ button*/
function inc_sec(){
	var payload = "second++";
	console.log("Publishing ", payload, " to ", w2b);
	client.publish(w2b, payload);
}


/* silence sound (and reset alarm if alarm went off); called on click for silence sound */
function silence_alarm(){
	var payload = "silence";
	console.log("Publishing ", payload, " to ", w2b);
	client.publish(w2b, payload);
}


/* play sound; called on click for play sound */
function play_sound(){
	var payload = "play sound";
	console.log("Publishing ", payload, " to ", w2b);
	client.publish(w2b, payload);
}


function onMessageArrived(topic, message) {
	console.log(topic, String(message));
  
	switch (topic) {
	  case hour_bd:
		hour = update(parseInt(message));
		break;
	  case min_bd:
		minute = update(parseInt(message));
		break;
	  case sec_bd:
		second = update(parseInt(message));
		break;
	  case mode_bd:
		mil_time = parseInt(message); // 0 = 12hr, 1 = 24hr
		break;
	  case am_or_pm:
		ampm = parseInt(message); // expect 0 = AM, 1 = PM
		break;
	  default:
		break;
	}
  
	// Refresh website clock
	Board_Time();
}


function Board_Time() {
	let period = "";

	// Decide AM/PM only from the topic
	if (mil_time == 0) {  
		if (ampm === 0) {
			period = "AM";
		} else if (ampm === 1) {
			period = "PM";
		}
	} 
	// 24-hour mode shows no AM/PM
	else if (mil_time == 1) {
		period = "";
	}

	document.getElementById("board-clock").innerText =
		hour + " : " + minute + " : " + second + " " + period;

	setTimeout(Board_Time, 1000);
}


function update(t) {
	if (t < 10) {
		return "0" + t;
	}
	else {
		return t;
	}
}