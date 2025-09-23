function decideOrientation(){
        console.log("decideOrientation")
        if(Config.screenorientation === "All" || Config.screenorientation === "automatic"){
            return Orientation.All
        }else if(Config.screenorientation === "Landscape"){
            return Orientation.Landscape
        }
        else if (Config.screenorientation === "Portrait"){
            return Orientation.Portrait
        }

}

