#include "config.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
////////////////////////////////////////////////////////////////////////////////
    Config::Config() : Parser(){
        parser->set_validate(true);
    }
////////////////////////////////////////////////////////////////////////////////
    Config::~Config(){
    }
////////////////////////////////////////////////////////////////////////////////
    bool
    Config::parse(const std::string& filename){
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
            parser->parse_file(filename);
            if(parser){
                //Walk the tree:
                const xmlpp::Node* pNode = parser->get_document()->get_root_node(); //deleted by DomParser.
//                print_node(pNode);
            }
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        }
        catch(const std::exception& ex){
            std::cout << "Exception caught: " << ex.what() << std::endl;
        }
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    }
////////////////////////////////////////////////////////////////////////////////
     void
     Config::Base_Image_Path(const std::string& text){
         /* ToDo Check access to path */
         _base_image_path->assign(text);
     }
////////////////////////////////////////////////////////////////////////////////
     std::string&
     Config::Base_Image_Path(){
         return *_base_image_path;
     }
////////////////////////////////////////////////////////////////////////////////
} // namespace Core
