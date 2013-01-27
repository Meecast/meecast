    function save_config_js(){
        var wgt_private_directory;
        var ConfigFile;
        Module.print(' Save Config JS111 ');

      //  Module.print(String.fromCharCode.apply(String, FS.analyzePath('config.xml').object.contents));
        try {
            tizen.filesystem.resolve('wgt-private',
                         function(dir){
                            wgt_private_directory = dir;
                            Module.print('\'wgt-private\' resolved ' + wgt_private_directory);

                            try {
                                Module.print("Dir path " + dir.path);
                                ConfigFile = dir.resolve('config.xml');
                                Module.print('Resolve file config.xml ');
                                try {
                                    // Module.print(String.fromCharCode.apply(String, FS.analyzePath('config.xml').object.contents));
                                    ConfigFile.openStream(
                                        'w', function(filetostream){
                                             filetostream.write(String.fromCharCode.apply(String, FS.analyzePath('config.xml').object.contents));
                                            filetostream.close();
                                         }, function(e){
                                            Module.print("Error" + e.message);
                                         }
                                      );
                                    Module.print('Save is done');
                                } catch (exc){
                                    Module.print('Write() exception:' + exc.message);
                                }
                            } catch (exc){
                                Module.print('Not resolve file config.xml ' + exc.message + '<br/>');
                            }
                         },
                         function(e){
                            Module.print("Error" + e.message);
                         } ,
                         'rw');
        } catch (exc){
              Module.print("Error" + exc.message);
        }
    };

    function  _currentstationname_js(){
        var current_station_name = Module.cwrap('current_station_name', 'string', []);
        document.getElementById('main_station_name').innerHTML = current_station_name();
    };

    function create_sources_list_js(){
        var create_sources_list = Module.cwrap('create_sources_list', 'string', []);
        Module.print("Data!!!!! :" + create_sources_list());
        var data = JSON.parse(create_sources_list());
        var text = "";
        for (var i in data) {
            text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"javascript:countries_page('"+ data[i] + "');\">" + data[i] + "</a></li>";
            //text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"javascript:countries_page(\"" + data[i] + "\");\">" + data[i] + "</a></li>";
//            text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"manage_country.html\">" + data[i] + "</a></li>";
        }
              Module.print("List1" + text);
        return text;
    };

    function create_countries_list_js(dbase){

        var create_countries_list = Module.cwrap('create_countries_list', 'string', ['string']);
        var data = JSON.parse(create_countries_list(dbase));
        var text = "";
        for (var i in data) {
            text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"javascript:regions_page('"+ data[i] + "');\">" + data[i] + "</a></li>";
            //text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"javascript:countries_page(\"" + data[i] + "\");\">" + data[i] + "</a></li>";
//            text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"manage_country.html\">" + data[i] + "</a></li>";
        }
             Module.print("List2" + text);
        return text;
    };

    function create_regions_list_js(country){

        var create_regions_list = Module.cwrap('create_regions_list', 'string', ['string']);
        var data = JSON.parse(create_regions_list(country));
        var text = "";
        for (var i in data) {
            text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"javascript:stations_page('"+ data[i] + "');\">" + data[i] + "</a></li>";
            //text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"javascript:countries_page(\"" + data[i] + "\");\">" + data[i] + "</a></li>";
//            text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"manage_country.html\">" + data[i] + "</a></li>";
        }
             Module.print("List3" + text);
        return text;
    };

//get_station_code_by_name
    function create_stations_list_js(country, region){

        var create_stations_list = Module.cwrap('create_stations_list', 'string', ['string', 'string']);
        var data = JSON.parse(create_stations_list(country, region));
        var text = "";
        for (var i in data) {
            text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"javascript:select_station('"+ data[i] + "');\">" + data[i] + "</a></li>";
            //text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"javascript:countries_page(\"" + data[i] + "\");\">" + data[i] + "</a></li>";
//            text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"manage_country.html\">" + data[i] + "</a></li>";
        }
             Module.print("List4" + text);
        return text;
    };

    function station_code_js(country, region, station){

        var station_code = Module.cwrap('station_code', 'string', ['string', 'string', 'string' ]);
        var data = station_code(country, region, station);
        Module.print("Code of station" + data);
        return data;
    };

    function save_station_js(source, country, region, station, code){

        var save_station = Module.cwrap('save_station', null, ['string', 'string', 'string', 'string', 'string']);
        save_station(source, country, region, station, code);
        return;
    };


   

    function stations_update_js(){
        var download_forecasts = Module.cwrap('download_forecasts', null, []);
        download_forecasts();
        return;
    };

   function stations_convert_js(){
        Module.print ('stations_convert_js');
        var convert_forecasts = Module.cwrap('convert_forecasts', null, []);
        convert_forecasts();
        return;
    };

    function  prepare_database_js(database_name){
        var WidgetDir;
        var ConfigFile;
        var DBFileFS;
        
        try {
            tizen.filesystem.resolve('wgt-package',
             function(dir){
                WidgetDir = dir;
                Module.print('\'wgt-package\' resolved ' + WidgetDir);

                    // Check file in original path 
                    ret = FS.analyzePath(database_name + ".db");
                    if (ret.exists) {
                        Module.print('Original database file is done');
                        $.mobile.changePage('manage_country.html'); //Change page view
                        return;
                    }

                    try {
                        Module.print("Dir path " + dir.path + " Database name " + database_name);
                        DBFile = dir.resolve('WebContent/' + database_name + ".db");
                        XMLFile = dir.resolve('WebContent/' + database_name + ".xml");

                        try {
                            DBFile.openStream(
                                'r',
                                function(fileStream) {
                                    var contents = fileStream.readBytes(fileStream.bytesAvailable);
                                    DBFileFS = FS.createDataFile('/', database_name + ".db", contents, true, true);
                                    fileStream.close();
                                    Module.print('Database file is done');
                                    $.mobile.changePage('manage_country.html'); //Change page view
                                },
                                function(e){
                                    Module.print("Error" + e.message);
                                }
                            );
                            XMLFile.openStream(
                                'r',
                                function(fileStream) {
                                    var contents = fileStream.readBytes(fileStream.bytesAvailable);
                                    XMLFileFS = FS.createDataFile('/', database_name + ".xml", contents, true, true);
                                    fileStream.close();
                                    Module.print('Xml file is done');
                                },
                                function(e){
                                    Module.print("Error" + e.message);
                                }
                            );
                        } catch (exc){
                            Module.print('read() exception:' + exc.message);
                        }
                    } catch (exc){
                        Module.print('Not resolve database file  ' + exc.message + '<br/>');
                    }
            },
             function(e){
                Module.print("Error" + e.message);
             } ,
             'r');
        } catch (exc) {
            Module.print('tizen.filesystem.resolve() exception: ' + exc.message + '<br/>');
         } 
    };


    function _prepare_config_js(){

        var wgt_private_directory;
        var ConfigFile;
        var ConfigFileFS;
        
        //dirname = Pointer_stringify(directory);

        Module.print(' Function prepareconfig11 ');
        var prepareconfig = Module.cwrap('prepareconfig', null, []);
        Module.print(' Function prepareconfig ' + prepareconfig);
        Module.print(' Try to resolve  directory ' + wgt_private_directory);
        try {
            tizen.filesystem.resolve('wgt-private',
                         function(dir){
                            wgt_private_directory = dir;
                            Module.print('\'wgt-private\' resolved ' + wgt_private_directory);

                            try {
                                Module.print("Dir path " + dir.path);
                                ConfigFile = dir.resolve('config.xml');
                                Module.print('Resolve file config.xml ');
                                try {
                                    ConfigFile.readAsText(
                                        function(contents) {
                                           ConfigFileFS = FS.createDataFile('/', 'config.xml', contents, true, true);
                                            Module.print('Config file config.xml is done');
                                            prepareconfig();
                                        },
                                        function(e){
                                            Module.print("Error" + e.message);
                                        }
                                    );
                                } catch (exc){
                                    Module.print('readAsText() exception:' + exc.message);
                                }
                            } catch (exc){
                                Module.print('Not resolve file config.xml ' + exc.message + '<br/>');
                                try {
                                    ConfigFile = dir.createFile('config.xml');
                                    Module.print('Create file config.xml ');
                                    Module.print(' Function11 ' );
                                   // ConfigFileFS = FS.createDataFile('/', 'config.xml', '', true, true);
                                    ConfigFileFS = Module["FS_createDataFile"]('/', 'config.xml', '', true, true);
                                    Module.print(' Function ');
                                    Module.print(' Function ' + prepareconfig);
                                    prepareconfig();
                                    Module.print('Create file config.xml 2222');
                                } catch (exc){
                                    Module.print('Not create2 file config.xml ' + exc.message + '<br/>');
                                }
                            }
                        },
                         function(e){
                            Module.print("Error" + e.message);
                            prepareconfig();
                         } ,
                         'rw');
         } catch (exc) {
            Module.print('tizen.filesystem.resolve() exception: ' + exc.message + '<br/>');
         } 
    };

