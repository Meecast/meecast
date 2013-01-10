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
               text = text + " <li><a href=\"file://index.html\">"+ data[i] + "</a></li>";
        }
        return text;
    },

    prepare_database_js: function(){

        var WidgetDir;
        var ConfigFile;
        var DBFileFS;
        
        var preparedatabase = Module.cwrap('preparedatabase', null, []);
        try {
            tizen.filesystem.resolve('wgt-package',
                         function(dir){
                            WidgetDir = dir;
                            Module.print('\'wgt-package\' resolved ' + WidgetDir);

//                            try {
//                                WidgetDir.listFiles(
//                                    function(files){
//                                  
//                                        var list = "";
//                                        for (i = 0 ; i < files.length ; i++) {
//                                            if (files[i].isDirectory == false)
//                                                    list += '<tr>' +
//                                                              '<td>' + files[i].fullPath + '</td>' + 
//                                                              '<td><button onclick="showDetailsClicked(\'' + files[i].name + '\');">Show</button></td>' +
//                                                              '<td><button onclick="deleteFileClicked(\'' + files[i].fullPath + '\');">Delete</button></td>' +
//                                                              '</tr>'; 
//                                        }
//
//
//
//                                            Module.print("File" + list);
//                                    },
//                                    function(e){
//                                            Module.print("Error" + e.message);
//                                    }
//                                );
//                            }catch(exc){
//
//                               Module.print('Not create file config.xml ' + exc.message + '<br/>');
//                            }
                            try {
                                Module.print("Dir path " + dir.path);
                                DBFile = dir.resolve('WebContent/weather.com.db');
                                Module.print('Resolve file database - Size: ' + DBFile.fileSize);

                                try {
                                    DBFile.openStream(
                                        'r',
                                        function(fileStream) {
                                            var contents = fileStream.readBytes(fileStream.bytesAvailable);
                                            DBFileFS = FS.createDataFile('/', 'weather.com.db', contents, true, true);
                                            fileStream.close();
                                            Module.print('Database file is done');
                                            preparedatabase();
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
                                try {
                                } catch (exc){
                                    Module.print('Not create file config.xml ' + exc.message + '<br/>');
                                }
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

