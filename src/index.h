const char page[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="utf-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<title>AC Control</title>
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<link rel='shortcut icon' type='image/x-icon' href='https://raw.githubusercontent.com/crankyoldgit/IRremoteESP8266/master/examples/Web-AC-control/data/favicon.ico' />
	<style type="text/css">
   	body { padding-top: 70px; }
    .btn {height: 40px;}
	</style>
	<!-- Font Awsome -->
    <link href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css" rel="stylesheet" integrity="sha384-wvfXpqpZZVQGK6TAh5PVlGOfQNHSoD2xbE+QkPxCAFlNEevoEH3Sl0sibVcOQVnN" crossorigin="anonymous">
	<!-- Latest compiled and minified CSS -->
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">
	<!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
	<!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
	<!--[if lt IE 9]>
		<script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
	   <script src="https://oss.maxcdn.com/libs/respond.js/1.3.0/respond.min.js"></script>
	<![endif]-->
	<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
    <script>
        var state = {}
        function updateStatus() {
        $.ajax({
            type: 'GET',
            url: "state",
            dataType: "json",
            data: [{
            name: "light",
            value: "1"
            }, ],
            success: function(data) {
            if (!data) {
                return;
            }
            state = data;
            if (state["power"] === true) {
                $("#power").text(" ON");
                $("#power-btn").addClass("btn-info");
                $("#power-btn").removeClass("btn-default");
            } else {
                $("#power").text(" OFF");
                $("#power-btn").addClass("btn-default");
                $("#power-btn").removeClass("btn-info");
            }
            //$("#target_temp").text(state["temp"] + "°C");
            $("#target_temp").text(state["temp"] + "°F");
            setModeColor(state["mode"]);
            setFanColor(state["fan"]);
            },
            error: function() {
            console.log('error getting state');
            },
            timeout: 1000
        });
        }
        updateStatus();
        function postData(t) {
        var e = new XMLHttpRequest;
        e.timeout = 2000;
        e.open("PUT", "state", !0);
        e.setRequestHeader("Content-Type", "application/json");
        console.log(JSON.stringify(t)), e.send(JSON.stringify(t));
        }
        function mode_onclick(mode) {
        state["mode"] = mode;
        setModeColor(mode);
        postData(state);
        }
        function setModeColor(mode) {
        $(".mode-btn").addClass("btn-default");
        $(".mode-btn").removeClass("btn-info");
        if (mode === 0) {
            $("#mode_auto").removeClass("btn-default");
            $("#mode_auto").addClass("btn-info");
            setFanColor(0);
            state["fan"] = 0;
        } else if (mode === 1) {
            $("#mode_cooling").removeClass("btn-default");
            $("#mode_cooling").addClass("btn-info");
        } else if (mode === 2) {
            $("#mode_dehum").removeClass("btn-default");
            $("#mode_dehum").addClass("btn-info");
        } else if (mode === 3) {
            $("#mode_heating").removeClass("btn-default");
            $("#mode_heating").addClass("btn-info");
        } else if (mode === 4) {
            $("#mode_fan").removeClass("btn-default");
            $("#mode_fan").addClass("btn-info");
        }
        }
        function setFanColor(fan) {
        if (fan == 0) {
            $("#fan_auto").removeClass("btn-default");
            $("#fan_auto").addClass("btn-info");
        } else {
            $("#fan_auto").removeClass("btn-info");
            $("#fan_auto").addClass("btn-default");
        }
        for (var i = 1; i <= 3; ++i) {
            if (i <= fan) {
            $("#fan_lvl_" + i).attr("src", "https://raw.githubusercontent.com/crankyoldgit/IRremoteESP8266/master/examples/Web-AC-control/data/level_" + i + "_on.svg");
            } else {
            $("#fan_lvl_" + i).attr("src", "https://raw.githubusercontent.com/crankyoldgit/IRremoteESP8266/master/examples/Web-AC-control/data/level_" + i + "_off.svg");
            }
        }
        }
        function fan_onclick(fan) {
        if (state["mode"] !== 0) {
            state["fan"] = fan;
            setFanColor(fan);
            postData(state);
        }
        }
        function power_onclick(power) {
        if (state["power"]) {
            state["power"] = false;
            $("#power").text(" OFF");
            $("#power-btn").removeClass("btn-info");
            $("#power-btn").addClass("btn-default");
        } else {
            state["power"] = true;
            $("#power").text(" ON");
            $("#power-btn").addClass("btn-info");
            $("#power-btn").removeClass("btn-default");
        }
        postData(state);
        }
        function temp_onclick(temp) {
        // val=[17,30]; // Celcius
        val=[62,86]; // Fahrenheit
        state["temp"] += temp;
        if (state["temp"] < val[0]) {
            state["temp"] = val[0];
        }
        if (state["temp"] > val[1]) {
            state["temp"] = val[1];
        }
        //$("#target_temp").text(state["temp"] + "°C");
        $("#target_temp").text(state["temp"] + "°F");
        postData(state);
        }
    </script>
</head>
	<!--page content-->
<body>
	<div class='container'>
		<nav class="navbar navbar-default navbar-fixed-top" role="navigation">
		  <div class="container-fluid">
			<div class="navbar-brand">
				<p>AC Control</p>
			</div>
			<p class="navbar-text navbar-right sr-only" id="spinner"><i class="fa fa-circle-o-notch fa-spin fa-lg"></i></p>
		  </div>
		</nav>

		<div  class="alert alert-danger sr-only" id="alert">
			<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
			<p></p>
		</div>

		<div class="container-fluid">
			<div class="row">
		  		<!-- ON/OFF -->
		  		<div class="col-sm-offset-10 col-sm-2 pull-right">
		  			<a id="power-btn" class="btn btn-default" onclick='power_onclick();'><i class="fa fa-power-off" style="font-size:1.6em;color:black;"></i><b style="font-size:1.5em;" id="power"> OFF</b></a>
		  		</div>
	  		</div>
			<br>
			<div class="row">
	   		<div class="col-md-4 col-sm-6">
	   			<!-- Mode -->
					<h4>Mode</h4>
					<div class="btn-group">
						<a class="btn btn-default mode-btn" id="mode_cooling" onclick='mode_onclick(1);'><i class="fa fa-asterisk fa-2x"></i></a>
						<a class="btn btn-default mode-btn" id="mode_dehum" 	onclick='mode_onclick(2);'><i class="fa fa-tint fa-2x"></i></a>
						<a class="btn btn-default mode-btn" id="mode_heating" onclick='mode_onclick(3);'><i class="fa fa-sun-o fa-2x"></i></a>
						<a class="btn btn-default mode-btn" id="mode_fan" 		onclick='mode_onclick(4);'><i class="fa fa-retweet fa-2x"></i></a>
						<a class="btn btn-default mode-btn" id="mode_auto" 	onclick='mode_onclick(0);'>Auto<!--<i class="fa fa-font fa-2x"></i>--></a>
					</div>
		  		</div>
		  		<div class="col-md-4 col-sm-6">
		  			<!-- Fan -->
					<h4>Fan</h4>
					<div class="btn-group">
						<a class="btn btn-default fan-btn" id="fan_auto" 	onclick='fan_onclick("0");'>Auto<!--<i class="fa fa-font fa-2x"></i>--></a>
						<a class="btn btn-default" onclick='fan_onclick(1);'><img src="https://raw.githubusercontent.com/crankyoldgit/IRremoteESP8266/master/examples/Web-AC-control/data/level_1_off.svg" height="29px" id="fan_lvl_1"></a>
						<a class="btn btn-default" onclick='fan_onclick(2);'><img src="https://raw.githubusercontent.com/crankyoldgit/IRremoteESP8266/master/examples/Web-AC-control/data/level_2_off.svg" height="29px" id="fan_lvl_2"></a>
						<a class="btn btn-default" onclick='fan_onclick(3);'><img src="https://raw.githubusercontent.com/crankyoldgit/IRremoteESP8266/master/examples/Web-AC-control/data/level_3_off.svg" height="29px" id="fan_lvl_3"></a>
					</div>
		  		</div>
		  	</div>
		  	<br>
		  	<br>
		  	<div class="row">
		  		<div class="col-md-4 col-sm-6" id="target_temp_col">

		  			<!--Target temperature-->
					<h4>Target temperature</h4>
					<div class="btn-group" >
						<a class="btn btn-default" style="font-size:2em;"><b id="target_temp"> ~ C</b></a>
					</div>

					<div class="btn-group-vertical">
						<a class="btn btn-default wing-btn" id="target_temp_up" onclick='temp_onclick(1);' style="font-size:1.7em;padding-top:0px;padding-bottom:0px;"><i class="fa fa-chevron-up"></i></a>
						<a class="btn btn-default wing-btn"	id="target_temp_down" onclick='temp_onclick(-1);' style="font-size:1.7em;padding-top:0px;padding-bottom:0px;"><i class="fa fa-chevron-down"></i></a>
					</div>
		  		</div>
	  		</div>
		</div>

	</div>
	<br>
</body>
</html>
)=====";