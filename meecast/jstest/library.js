mergeInto(LibraryManager.library, {
    save_config_js: function(){
        var documentsDir;
        var ConfigFile;
        Module.print(' Save Config JS ');

      //  Module.print(String.fromCharCode.apply(String, FS.analyzePath('config.xml').object.contents));
        try {
            tizen.filesystem.resolve('wgt-private',
                         function(dir){
                            documentsDir = dir;
                            Module.print('\'wgt-private\' resolved ' + documentsDir);

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
    },

    currentstationname_js: function(){
        var current_station_name = Module.cwrap('current_station_name', 'string', []);
        document.getElementById('main_station_name').innerHTML = current_station_name();
    },

    create_sources_list_js: function(){
        var create_sources_list = Module.cwrap('create_sources_list', 'string', []);
        Module.print("Data!!!!! :" + create_sources_list());
        var data = JSON.parse(create_sources_list());
        var text = "";
        for (var i in data) {
            text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"javascript:countries_page('"+ data[i] + "');\">" + data[i] + "</a></li>";
            //text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"javascript:countries_page(\"" + data[i] + "\");\">" + data[i] + "</a></li>";
//            text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"manage_country.html\">" + data[i] + "</a></li>";
        }
              Module.print("List" + text);
        return text;
    },
    create_countries_list_js: function(){

        var create_countries_list = Module.cwrap('create_countries_list', 'string', []);
        var data = JSON.parse(create_countries_list());
        var text = "";
        for (var i in data) {
            text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"javascript:regions_page('"+ data[i] + "');\">" + data[i] + "</a></li>";
            //text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"javascript:countries_page(\"" + data[i] + "\");\">" + data[i] + "</a></li>";
//            text = text + " <li id=\"" + data[i] + "\"  data-icon=\"arrow-r\" data-iconpos=\"right\" ><a  href=\"manage_country.html\">" + data[i] + "</a></li>";
        }
             Module.print("List" + text);
        return text;
    },

    prepare_database_js: function(database_name){

        var WidgetDir;
        var ConfigFile;
        var DBFileFS;
        
        try {
            tizen.filesystem.resolve('wgt-package',
             function(dir){
                WidgetDir = dir;
                Module.print('\'wgt-package\' resolved ' + WidgetDir);

                    // Check file in original path 
                    ret = FS.analyzePath(database_name);
                    if (ret.exists) {
                        Module.print('Original database file is done');
                        $.mobile.changePage('manage_country.html'); //Change page view

                        return;
                    }

                    try {
                        Module.print("Dir path " + dir.path + " Database name " + database_name);
                        DBFile = dir.resolve('WebContent/' + database_name);

                        try {
                            DBFile.openStream(
                                'r',
                                function(fileStream) {
                                    var contents = fileStream.readBytes(fileStream.bytesAvailable);
                                    DBFileFS = FS.createDataFile('/', database_name, contents, true, true);
                                    fileStream.close();
                                    Module.print('Database file is done');
                                    $.mobile.changePage('manage_country.html'); //Change page view
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
                prepareconfig();
             } ,
             'r');
        } catch (exc) {
            Module.print('tizen.filesystem.resolve() exception: ' + exc.message + '<br/>');
         } 
    },

    prepare_config_js: function(){

        var documentsDir;
        var ConfigFile;
        var ConfigFileFS;
        
        //dirname = Pointer_stringify(directory);

        var prepareconfig = Module.cwrap('prepareconfig', null, []);
        Module.print(' Try to resolve  directory ' + documentsDir);
        try {
            tizen.filesystem.resolve('wgt-private',
                         function(dir){
                            documentsDir = dir;
                            Module.print('\'wgt-private\' resolved ' + documentsDir);

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
                                   // Module["FS_createDataFile"]
                                    ConfigFileFS = FS.createDataFile('/', 'config.xml', '', true, true);
                                    prepareconfig();
                                } catch (exc){
                                    Module.print('Not create file config.xml ' + exc.message + '<br/>');
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
    }
});

