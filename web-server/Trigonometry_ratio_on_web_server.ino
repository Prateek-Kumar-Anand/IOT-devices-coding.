<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Trig & Inverse Trig Graphs</title>
<style>
body{
    font-family: Arial, sans-serif;
    text-align:center;
}
canvas{
    border:1px solid black;
    margin-top:10px;
}
select{
    padding:8px;
    font-size:16px;
}
</style>
</head>
<body>
<h2>Trigonometric & Inverse Trigonometric Graphs</h2>
<select id="func" onchange="drawGraph()">
    <option value="sin">sin(x)</option>
    <option value="cos">cos(x)</option>
    <option value="tan">tan(x)</option>
    <option value="cosec">cosec(x)</option>
    <option value="sec">sec(x)</option>
    <option value="cot">cot(x)</option>
    <option value="asin">sin⁻¹(x)</option>
    <option value="acos">cos⁻¹(x)</option>
    <option value="atan">tan⁻¹(x)</option>
    <option value="acosec">cosec⁻¹(x)</option>
    <option value="asec">sec⁻¹(x)</option>
    <option value="acot">cot⁻¹(x)</option>
</select>
<br>
<canvas id="graph" width="1000" height="500"></canvas>
<script>
const canvas = document.getElementById("graph");
const ctx = canvas.getContext("2d");
function drawAxes()
{
    ctx.strokeStyle = "black";

    ctx.beginPath();
    ctx.moveTo(0,250);
    ctx.lineTo(1000,250);

    ctx.moveTo(500,0);
    ctx.lineTo(500,500);

    ctx.stroke();
}
function getValue(type,x)
{
    switch(type)
    {
        case "sin": return Math.sin(x);
        case "cos": return Math.cos(x);
        case "tan":
            return Math.abs(Math.tan(x))>10 ? NaN : Math.tan(x);

        case "cosec":
            let s=Math.sin(x);
            return Math.abs(s)<0.05 ? NaN : 1/s;

        case "sec":
            let c=Math.cos(x);
            return Math.abs(c)<0.05 ? NaN : 1/c;

        case "cot":
            let t=Math.tan(x);
            return Math.abs(t)<0.05 ? NaN : 1/t;

        case "asin":
            return (x<-1 || x>1) ? NaN : Math.asin(x);

        case "acos":
            return (x<-1 || x>1) ? NaN : Math.acos(x);

        case "atan":
            return Math.atan(x);

        case "acosec":
            return Math.abs(x)<1 ? NaN : Math.asin(1/x);

        case "asec":
            return Math.abs(x)<1 ? NaN : Math.acos(1/x);

        case "acot":
            return Math.atan(1/x);

        default:
            return NaN;
    }
}

function drawGraph()
{
    ctx.clearRect(0,0,canvas.width,canvas.height);

    drawAxes();

    let type = document.getElementById("func").value;

    ctx.strokeStyle = "red";
    ctx.lineWidth = 2;
    ctx.beginPath();

    let first=true;

    for(let px=0; px<1000; px++)
    {
        let x=(px-500)/50;

        let y=getValue(type,x);

        if(isNaN(y) || !isFinite(y))
        {
            first=true;
            continue;
        }

        let py=250-y*50;

        if(first)
        {
            ctx.moveTo(px,py);
            first=false;
        }
        else
        {
            ctx.lineTo(px,py);
        }
    }

    ctx.stroke();
}

drawGraph();

</script>

</body>
</html>
