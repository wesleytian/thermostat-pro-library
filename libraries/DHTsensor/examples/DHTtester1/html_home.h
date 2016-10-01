const char html[] = R"(

<!DOCTYPE html>
<html>
<head>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>

</head>


<body>
<h1>Settings</h1>

<p>Set the high limit: Please input a number between -40F and 176F:</p>

<form name="myform" action="set" method="GET">
<input name="high" id="hightempf">

<button type="button" onclick="myFunctionHighTempF() ">Submit</button>

<p id="hitempf"></p>

<p>Set the low limit: Please input a number between -40F and 176F:</p>

<input name="low" id="lowtempf">

<button type="button" onclick="myFunctionLowTempF() ">Submit</button>

</form>
<p id="lotempf"></p>


<script>
function myFunctionHighTempF() {
    var x, text;

    // Get the value of the input field with id="numb"
    x = document.getElementById("hightempf").value;

    // If x is Not a Number or less than one or greater than 10
    if (isNaN(x) || x < -40 || x > 176 || !x) {
        text = "Input not valid";
        return;
    } else {
        text = "High temperature threshold set.";
        highHeatLimit = x;
    }
    document.getElementById("hitempf").innerHTML = text;
    document.myform.submit();

}
</script>

<script>
function myFunctionLowTempF() {
    var x, text;

    // Get the value of the input field with id="numb"
    x = document.getElementById("lowtempf").value;

    // If x is Not a Number or less than one or greater than 10
    if (isNaN(x) || x < -40 || x > 176 || !x) {
        text = "Input not valid";
        return;
    } else {
        text = "Low temperature threshold set.";
        lowHeatLimit = x;
    }
    document.getElementById("lotempf").innerHTML = text;

    document.myform.submit();
}
</script>


</body>
</html>

)";
