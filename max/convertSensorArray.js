inlets = 1;
outlets = 1;

var outVal = new Array();

function list(val)
{
    if(arguments.length==46)
    {
        outVal[0] = arguments[0]; // keep bank id
        var j = 1;
        for(i=0; i<5; i++){ // iterate through 5 sensor rays
            for(sensorId = 0; sensorId < 9; sensorId++){ // iterate through 9 sensors
                 outVal[1+sensorId*5+4-i] = arguments[j];
                  j++;
            }
        }
        outlet(0,outVal);
    }
}