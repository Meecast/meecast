<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="page">
    <name>index</name>
    <title>libxml++ Reference Manual</title>
    <filename>index</filename>
    <docanchor file="index">features</docanchor>
    <docanchor file="index">basics</docanchor>
    <docanchor file="index">description</docanchor>
  </compound>
  <compound kind="class">
    <name>sigc::trackable</name>
    <filename>classsigc_1_1trackable.html</filename>
  </compound>
  <compound kind="class">
    <name>std::exception</name>
    <filename>classstd_1_1exception.html</filename>
  </compound>
  <compound kind="namespace">
    <name>xmlpp</name>
    <filename>namespacexmlpp.html</filename>
    <class kind="class">xmlpp::Attribute</class>
    <class kind="class">xmlpp::AttributeDeclaration</class>
    <class kind="class">xmlpp::AttributeNode</class>
    <class kind="class">xmlpp::Document</class>
    <class kind="class">xmlpp::Dtd</class>
    <class kind="class">xmlpp::KeepBlanks</class>
    <class kind="class">xmlpp::NonCopyable</class>
    <class kind="class">xmlpp::Schema</class>
    <class kind="class">xmlpp::exception</class>
    <class kind="class">xmlpp::parse_error</class>
    <class kind="class">xmlpp::validity_error</class>
    <class kind="class">xmlpp::internal_error</class>
    <class kind="class">xmlpp::IStreamParserInputBuffer</class>
    <class kind="class">xmlpp::OutputBuffer</class>
    <class kind="class">xmlpp::OStreamOutputBuffer</class>
    <class kind="class">xmlpp::ParserInputBuffer</class>
    <class kind="class">xmlpp::CdataNode</class>
    <class kind="class">xmlpp::CommentNode</class>
    <class kind="class">xmlpp::ContentNode</class>
    <class kind="class">xmlpp::Element</class>
    <class kind="class">xmlpp::EntityDeclaration</class>
    <class kind="class">xmlpp::EntityReference</class>
    <class kind="class">xmlpp::Node</class>
    <class kind="class">xmlpp::ProcessingInstructionNode</class>
    <class kind="class">xmlpp::TextNode</class>
    <class kind="class">xmlpp::Parser</class>
    <class kind="class">xmlpp::SaxParser</class>
    <class kind="class">xmlpp::DomParser</class>
    <class kind="class">xmlpp::TextReader</class>
    <class kind="class">xmlpp::DtdValidator</class>
    <class kind="class">xmlpp::SchemaValidator</class>
    <class kind="class">xmlpp::Validator</class>
    <member kind="typedef">
      <type>std::vector&lt; Node * &gt;</type>
      <name>NodeSet</name>
      <anchorfile>namespacexmlpp.html</anchorfile>
      <anchor>a649baf2bb62274444784ffb180b3f34c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>XmlEntityType</name>
      <anchorfile>namespacexmlpp.html</anchorfile>
      <anchor>a0fa47f0fb103cf9ea460a2fef3f5be49</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>XML_INTERNAL_GENERAL_ENTITY</name>
      <anchorfile>namespacexmlpp.html</anchorfile>
      <anchor>a0fa47f0fb103cf9ea460a2fef3f5be49a9b66b089105754e77f17310b6f6f343f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>XML_EXTERNAL_GENERAL_PARSED_ENTITY</name>
      <anchorfile>namespacexmlpp.html</anchorfile>
      <anchor>a0fa47f0fb103cf9ea460a2fef3f5be49a626dc4ebe4c379e02e282b2d80590986</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>XML_EXTERNAL_GENERAL_UNPARSED_ENTITY</name>
      <anchorfile>namespacexmlpp.html</anchorfile>
      <anchor>a0fa47f0fb103cf9ea460a2fef3f5be49a03b6660b9e31281fa6d4b3dab660e10f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>XML_INTERNAL_PARAMETER_ENTITY</name>
      <anchorfile>namespacexmlpp.html</anchorfile>
      <anchor>a0fa47f0fb103cf9ea460a2fef3f5be49a45b52d592f43e7d2b229f316e784ee23</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>XML_EXTERNAL_PARAMETER_ENTITY</name>
      <anchorfile>namespacexmlpp.html</anchorfile>
      <anchor>a0fa47f0fb103cf9ea460a2fef3f5be49a13726f7ac203efa91a1203c70ae80729</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>XML_INTERNAL_PREDEFINED_ENTITY</name>
      <anchorfile>namespacexmlpp.html</anchorfile>
      <anchor>a0fa47f0fb103cf9ea460a2fef3f5be49a374b57b001a4590bc237822002038628</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>format_xml_error</name>
      <anchorfile>namespacexmlpp.html</anchorfile>
      <anchor>ae0b6d16fb8cb19cf9055a4f9df06bc3a</anchor>
      <arglist>(const _xmlError *error=0)</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>format_xml_parser_error</name>
      <anchorfile>namespacexmlpp.html</anchorfile>
      <anchor>a45a70622ecc99bda595ecc83991de21c</anchor>
      <arglist>(const _xmlParserCtxt *parser_context)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::Attribute</name>
    <filename>classxmlpp_1_1Attribute.html</filename>
    <base>xmlpp::Node</base>
    <member kind="function">
      <type></type>
      <name>Attribute</name>
      <anchorfile>classxmlpp_1_1Attribute.html</anchorfile>
      <anchor>a1aa5294ec06fa7d5c1198e2c47d5838c</anchor>
      <arglist>(_xmlNode *node)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Attribute</name>
      <anchorfile>classxmlpp_1_1Attribute.html</anchorfile>
      <anchor>af44eddff10b76c29d48cb98926b4c367</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_name</name>
      <anchorfile>classxmlpp_1_1Attribute.html</anchorfile>
      <anchor>a553e7a694039b23150c27bfc05af2d60</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_value</name>
      <anchorfile>classxmlpp_1_1Attribute.html</anchorfile>
      <anchor>ac871c3356cabd34e849968ad9d79e5fd</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_value</name>
      <anchorfile>classxmlpp_1_1Attribute.html</anchorfile>
      <anchor>a24dfc8f17dad4ff629241af54d12ba78</anchor>
      <arglist>(const Glib::ustring &amp;value)</arglist>
    </member>
    <member kind="function">
      <type>_xmlAttr *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1Attribute.html</anchorfile>
      <anchor>a1bdce49b8e5f118373732b4a3e14b842</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const _xmlAttr *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1Attribute.html</anchorfile>
      <anchor>a9e6ae08227e9a81dcd18ef5cbf820dc0</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::AttributeDeclaration</name>
    <filename>classxmlpp_1_1AttributeDeclaration.html</filename>
    <base>xmlpp::Attribute</base>
    <member kind="function">
      <type></type>
      <name>AttributeDeclaration</name>
      <anchorfile>classxmlpp_1_1AttributeDeclaration.html</anchorfile>
      <anchor>a6d7dd6ccbe4ad325ccb053a252b10c9c</anchor>
      <arglist>(_xmlNode *node)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~AttributeDeclaration</name>
      <anchorfile>classxmlpp_1_1AttributeDeclaration.html</anchorfile>
      <anchor>a1e427ebd8c75f5e15b6b7939e55cc30c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_value</name>
      <anchorfile>classxmlpp_1_1AttributeDeclaration.html</anchorfile>
      <anchor>aaf0072ae8ed02761af114f4afe885192</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>_xmlAttribute *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1AttributeDeclaration.html</anchorfile>
      <anchor>ab1bc51b1d5122eb25f1fd2cb4ceb3065</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const _xmlAttribute *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1AttributeDeclaration.html</anchorfile>
      <anchor>a4ba1602b6a48b849c2cf7a0b3414f5ef</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::AttributeNode</name>
    <filename>classxmlpp_1_1AttributeNode.html</filename>
    <base>xmlpp::Attribute</base>
    <member kind="function">
      <type></type>
      <name>AttributeNode</name>
      <anchorfile>classxmlpp_1_1AttributeNode.html</anchorfile>
      <anchor>a0af899521d66b09398deb5fa7fd26035</anchor>
      <arglist>(_xmlNode *node)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~AttributeNode</name>
      <anchorfile>classxmlpp_1_1AttributeNode.html</anchorfile>
      <anchor>a6321127fb72b11784fd6a16f17d678bd</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::Document</name>
    <filename>classxmlpp_1_1Document.html</filename>
    <base protection="private">xmlpp::NonCopyable</base>
    <member kind="function">
      <type></type>
      <name>Document</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>a8c5c6692e195dcf3aee88dc492f9efed</anchor>
      <arglist>(const Glib::ustring &amp;version=&quot;1.0&quot;)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Document</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>a77c35c594fc9995970ae9c4f9d77496b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_encoding</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>a9527fadaffff6138b0b35050370a0e3d</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Dtd *</type>
      <name>get_internal_subset</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>ae27fb04b1760bcaa58f7e329c98fae47</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_internal_subset</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>a714dc6c330ae6e00fb285606b8b849a4</anchor>
      <arglist>(const Glib::ustring &amp;name, const Glib::ustring &amp;external_id, const Glib::ustring &amp;system_id)</arglist>
    </member>
    <member kind="function">
      <type>Element *</type>
      <name>get_root_node</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>a6f4ebd0db42eeca823517dd1bd56d009</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Element *</type>
      <name>create_root_node</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>a8552403b511567241856148a6040be23</anchor>
      <arglist>(const Glib::ustring &amp;name, const Glib::ustring &amp;ns_uri=Glib::ustring(), const Glib::ustring &amp;ns_prefix=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>Element *</type>
      <name>create_root_node_by_import</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>a8085d5e40537e2a674063cbddb39f0b8</anchor>
      <arglist>(const Node *node, bool recursive=true)</arglist>
    </member>
    <member kind="function">
      <type>CommentNode *</type>
      <name>add_comment</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>a2d5ebd0f41215d54b9c96dd4ddb49ede</anchor>
      <arglist>(const Glib::ustring &amp;content)</arglist>
    </member>
    <member kind="function">
      <type>ProcessingInstructionNode *</type>
      <name>add_processing_instruction</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>aeaa438c9764e7d41f3d2f63ace6dab81</anchor>
      <arglist>(const Glib::ustring &amp;name, const Glib::ustring &amp;content)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write_to_file</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>adbbcae29bde29501f3dfff71f2e81da2</anchor>
      <arglist>(const Glib::ustring &amp;filename, const Glib::ustring &amp;encoding=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write_to_file_formatted</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>ae54075502d81e9d8448e6820bc16eb34</anchor>
      <arglist>(const Glib::ustring &amp;filename, const Glib::ustring &amp;encoding=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>write_to_string</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>ab63344d932c309062769fd98585e9bc1</anchor>
      <arglist>(const Glib::ustring &amp;encoding=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>write_to_string_formatted</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>ac1246acf8c7b51855e9eb12eecefa294</anchor>
      <arglist>(const Glib::ustring &amp;encoding=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write_to_stream</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>a55e593694bdcf67f667f1dbe52cd0794</anchor>
      <arglist>(std::ostream &amp;output, const Glib::ustring &amp;encoding=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write_to_stream_formatted</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>add90b2d2d0a837df3f3daec6bccaad2c</anchor>
      <arglist>(std::ostream &amp;output, const Glib::ustring &amp;encoding=Glib::ustring())</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>set_entity_declaration</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>ad56a99bd883672ba5eac51a545fada40</anchor>
      <arglist>(const Glib::ustring &amp;name, XmlEntityType type, const Glib::ustring &amp;publicId, const Glib::ustring &amp;systemId, const Glib::ustring &amp;content)</arglist>
    </member>
    <member kind="function">
      <type>_xmlDoc *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>a1ebb73b9ac51ca929bfd38f359964a59</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const _xmlDoc *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>a4cbdce4b14db6b3a393346b5e3034886</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>Document</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>ac18dbf6e4c404dac08a0784c553c5e88</anchor>
      <arglist>(_xmlDoc *doc)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>_xmlEntity *</type>
      <name>get_entity</name>
      <anchorfile>classxmlpp_1_1Document.html</anchorfile>
      <anchor>a6772b76ee685c3755f8a19ce19f48f35</anchor>
      <arglist>(const Glib::ustring &amp;name)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::Dtd</name>
    <filename>classxmlpp_1_1Dtd.html</filename>
    <member kind="function">
      <type></type>
      <name>Dtd</name>
      <anchorfile>classxmlpp_1_1Dtd.html</anchorfile>
      <anchor>a654e86d880ec1e2f4eaa043b580083bc</anchor>
      <arglist>(_xmlDtd *dtd)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Dtd</name>
      <anchorfile>classxmlpp_1_1Dtd.html</anchorfile>
      <anchor>a2d161935b1c13bab000b2279f60c696b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_name</name>
      <anchorfile>classxmlpp_1_1Dtd.html</anchorfile>
      <anchor>a972abba9bcf1f84ceec40b282332db29</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_external_id</name>
      <anchorfile>classxmlpp_1_1Dtd.html</anchorfile>
      <anchor>afedd938e1bf67907ad8ddbfac74ac6e5</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_system_id</name>
      <anchorfile>classxmlpp_1_1Dtd.html</anchorfile>
      <anchor>ae8de041e1374791094d7c5f06a7b86aa</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>_xmlDtd *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1Dtd.html</anchorfile>
      <anchor>ac11f222663c0732d789a7d28353fbf96</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const _xmlDtd *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1Dtd.html</anchorfile>
      <anchor>af01e0449f41509e1e5a32ee13d3b27a3</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::KeepBlanks</name>
    <filename>classxmlpp_1_1KeepBlanks.html</filename>
    <member kind="function">
      <type></type>
      <name>KeepBlanks</name>
      <anchorfile>classxmlpp_1_1KeepBlanks.html</anchorfile>
      <anchor>a516fa3293bbf39fc3942dee06fec3385</anchor>
      <arglist>(bool value)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~KeepBlanks</name>
      <anchorfile>classxmlpp_1_1KeepBlanks.html</anchorfile>
      <anchor>abccd950634d0047be5d80486cd0824ad</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const bool</type>
      <name>Default</name>
      <anchorfile>classxmlpp_1_1KeepBlanks.html</anchorfile>
      <anchor>a71753ddbdcfb79fdead46b0123bc22dd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::NonCopyable</name>
    <filename>classxmlpp_1_1NonCopyable.html</filename>
    <member kind="function" protection="protected">
      <type></type>
      <name>NonCopyable</name>
      <anchorfile>classxmlpp_1_1NonCopyable.html</anchorfile>
      <anchor>a77c785ff1149642caa3058f49223f112</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual</type>
      <name>~NonCopyable</name>
      <anchorfile>classxmlpp_1_1NonCopyable.html</anchorfile>
      <anchor>a14e2e0391beb4b8f20f5d51e8a253d63</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::Schema</name>
    <filename>classxmlpp_1_1Schema.html</filename>
    <base protection="private">xmlpp::NonCopyable</base>
    <member kind="function">
      <type></type>
      <name>Schema</name>
      <anchorfile>classxmlpp_1_1Schema.html</anchorfile>
      <anchor>a772d929c2cce3943a1d605ff2069cc68</anchor>
      <arglist>(_xmlSchema *schema)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Schema</name>
      <anchorfile>classxmlpp_1_1Schema.html</anchorfile>
      <anchor>aedaae0872aacd4ba046c6e7804e4d52a</anchor>
      <arglist>(Document *document=0, bool embed=false)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Schema</name>
      <anchorfile>classxmlpp_1_1Schema.html</anchorfile>
      <anchor>acd93d7286aabea897cf76420a33e0ef2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>set_document</name>
      <anchorfile>classxmlpp_1_1Schema.html</anchorfile>
      <anchor>a2685225a63de28093f670f0a65020051</anchor>
      <arglist>(Document *document=0, bool embed=false)</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_name</name>
      <anchorfile>classxmlpp_1_1Schema.html</anchorfile>
      <anchor>a1136d4a1c052f450e1d2153234e1ca31</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_target_namespace</name>
      <anchorfile>classxmlpp_1_1Schema.html</anchorfile>
      <anchor>a2fd2cffa8010166516c25d957b2fbe84</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_version</name>
      <anchorfile>classxmlpp_1_1Schema.html</anchorfile>
      <anchor>a948b90685ae8f394be195effac10f502</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Document *</type>
      <name>get_document</name>
      <anchorfile>classxmlpp_1_1Schema.html</anchorfile>
      <anchor>a1dea312fc3284d60de36bd099399e8bb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const Document *</type>
      <name>get_document</name>
      <anchorfile>classxmlpp_1_1Schema.html</anchorfile>
      <anchor>ad8504ecb8acc821fcd3113d6ccc4bdb4</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>_xmlSchema *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1Schema.html</anchorfile>
      <anchor>ad200dae863d27fcdeccdba9653df8933</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const _xmlSchema *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1Schema.html</anchorfile>
      <anchor>ad2239be7d5ab5277076a07053687d903</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>release_underlying</name>
      <anchorfile>classxmlpp_1_1Schema.html</anchorfile>
      <anchor>a95292e2dad9af680c7741df4194411e6</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::exception</name>
    <filename>classxmlpp_1_1exception.html</filename>
    <base>std::exception</base>
    <member kind="function">
      <type></type>
      <name>exception</name>
      <anchorfile>classxmlpp_1_1exception.html</anchorfile>
      <anchor>aec1bbf4a42dd2ed44d269a3c31c14124</anchor>
      <arglist>(const Glib::ustring &amp;message)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~exception</name>
      <anchorfile>classxmlpp_1_1exception.html</anchorfile>
      <anchor>a4d0c3298c1bc27b4e2fdc6152330e760</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const char *</type>
      <name>what</name>
      <anchorfile>classxmlpp_1_1exception.html</anchorfile>
      <anchor>a0427039fbb35dc2156fcff024880b081</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>Raise</name>
      <anchorfile>classxmlpp_1_1exception.html</anchorfile>
      <anchor>aaa94eb2f3816552fb7ec9014b6c79807</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual exception</type>
      <name>Clone</name>
      <anchorfile>classxmlpp_1_1exception.html</anchorfile>
      <anchor>afcbd398f555b5c18d130ae65db015a24</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::parse_error</name>
    <filename>classxmlpp_1_1parse__error.html</filename>
    <base>xmlpp::exception</base>
    <member kind="function">
      <type></type>
      <name>parse_error</name>
      <anchorfile>classxmlpp_1_1parse__error.html</anchorfile>
      <anchor>a3b393340cea80548da8470006f1bcb9b</anchor>
      <arglist>(const Glib::ustring &amp;message)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~parse_error</name>
      <anchorfile>classxmlpp_1_1parse__error.html</anchorfile>
      <anchor>ae516bd60f15f50103f152e386f70ff75</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>Raise</name>
      <anchorfile>classxmlpp_1_1parse__error.html</anchorfile>
      <anchor>a12f18b563758bd0bdc73ac6019f2e2f6</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual exception *</type>
      <name>Clone</name>
      <anchorfile>classxmlpp_1_1parse__error.html</anchorfile>
      <anchor>adac842898e676e64e8c7ca3174f7db2f</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::validity_error</name>
    <filename>classxmlpp_1_1validity__error.html</filename>
    <base>xmlpp::parse_error</base>
    <member kind="function">
      <type></type>
      <name>validity_error</name>
      <anchorfile>classxmlpp_1_1validity__error.html</anchorfile>
      <anchor>a096d4cb8a9ec64d5d0203439400fd7fd</anchor>
      <arglist>(const Glib::ustring &amp;message)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~validity_error</name>
      <anchorfile>classxmlpp_1_1validity__error.html</anchorfile>
      <anchor>ab5350778036c60aa71d0b36759e9cee9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>Raise</name>
      <anchorfile>classxmlpp_1_1validity__error.html</anchorfile>
      <anchor>ac97e96c3d23357d5a640544aa7aa0c46</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual exception *</type>
      <name>Clone</name>
      <anchorfile>classxmlpp_1_1validity__error.html</anchorfile>
      <anchor>aa38252e9dff7e22bfdf94365c61b8c21</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::internal_error</name>
    <filename>classxmlpp_1_1internal__error.html</filename>
    <base>xmlpp::exception</base>
    <member kind="function">
      <type></type>
      <name>internal_error</name>
      <anchorfile>classxmlpp_1_1internal__error.html</anchorfile>
      <anchor>ac8c22422363ebe809517d4813903ef43</anchor>
      <arglist>(const Glib::ustring &amp;message)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~internal_error</name>
      <anchorfile>classxmlpp_1_1internal__error.html</anchorfile>
      <anchor>a83d7e81fd0394ae91fa3f37a3552461c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>Raise</name>
      <anchorfile>classxmlpp_1_1internal__error.html</anchorfile>
      <anchor>a15044cea719fb2bbb482efc597a284f0</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual exception</type>
      <name>Clone</name>
      <anchorfile>classxmlpp_1_1internal__error.html</anchorfile>
      <anchor>aaec2e497c76cf6418ecff03e8f76510b</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::IStreamParserInputBuffer</name>
    <filename>classxmlpp_1_1IStreamParserInputBuffer.html</filename>
    <base>xmlpp::ParserInputBuffer</base>
    <member kind="function">
      <type></type>
      <name>IStreamParserInputBuffer</name>
      <anchorfile>classxmlpp_1_1IStreamParserInputBuffer.html</anchorfile>
      <anchor>aba70d93277d719a332b1a75fc8cf221c</anchor>
      <arglist>(std::istream &amp;input)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~IStreamParserInputBuffer</name>
      <anchorfile>classxmlpp_1_1IStreamParserInputBuffer.html</anchorfile>
      <anchor>a4ae4d9a34cb47a73590dff158206ab83</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::OutputBuffer</name>
    <filename>classxmlpp_1_1OutputBuffer.html</filename>
    <base>xmlpp::NonCopyable</base>
    <member kind="function">
      <type></type>
      <name>OutputBuffer</name>
      <anchorfile>classxmlpp_1_1OutputBuffer.html</anchorfile>
      <anchor>a2e00dabdec3e40b837d2b9ecb1d9fbcd</anchor>
      <arglist>(const Glib::ustring &amp;encoding=Glib::ustring())</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~OutputBuffer</name>
      <anchorfile>classxmlpp_1_1OutputBuffer.html</anchorfile>
      <anchor>a02ddd3e8891f92d6518252f6326ed2a9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>_xmlOutputBuffer *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1OutputBuffer.html</anchorfile>
      <anchor>a2de0e355037956329d70b990d7bc79f5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const _xmlOutputBuffer *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1OutputBuffer.html</anchorfile>
      <anchor>a6dbd7fc5ab8af66e751e0209aa3a0346</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::OStreamOutputBuffer</name>
    <filename>classxmlpp_1_1OStreamOutputBuffer.html</filename>
    <base>xmlpp::OutputBuffer</base>
    <member kind="function">
      <type></type>
      <name>OStreamOutputBuffer</name>
      <anchorfile>classxmlpp_1_1OStreamOutputBuffer.html</anchorfile>
      <anchor>a94a071df974d7fb7065f11c63d707604</anchor>
      <arglist>(std::ostream &amp;output, const Glib::ustring &amp;encoding=Glib::ustring())</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~OStreamOutputBuffer</name>
      <anchorfile>classxmlpp_1_1OStreamOutputBuffer.html</anchorfile>
      <anchor>af3bce7e0789eae3605ecd72103cf4a96</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::ParserInputBuffer</name>
    <filename>classxmlpp_1_1ParserInputBuffer.html</filename>
    <base>xmlpp::NonCopyable</base>
    <member kind="function">
      <type></type>
      <name>ParserInputBuffer</name>
      <anchorfile>classxmlpp_1_1ParserInputBuffer.html</anchorfile>
      <anchor>a546358e9f85fd99b4ac4e1a2b1844ce5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ParserInputBuffer</name>
      <anchorfile>classxmlpp_1_1ParserInputBuffer.html</anchorfile>
      <anchor>ad48655eb2fb4d085a7f2531aa8979b75</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>_xmlParserInputBuffer *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1ParserInputBuffer.html</anchorfile>
      <anchor>acb105eb192e5532e21431a82ff46bb82</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const _xmlParserInputBuffer *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1ParserInputBuffer.html</anchorfile>
      <anchor>acf1c0eacf5b5d327a864f5339818d6bf</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::CdataNode</name>
    <filename>classxmlpp_1_1CdataNode.html</filename>
    <base>xmlpp::ContentNode</base>
    <member kind="function">
      <type></type>
      <name>CdataNode</name>
      <anchorfile>classxmlpp_1_1CdataNode.html</anchorfile>
      <anchor>a150933b23cfa1a5c0fc5fffd433e7194</anchor>
      <arglist>(_xmlNode *node)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~CdataNode</name>
      <anchorfile>classxmlpp_1_1CdataNode.html</anchorfile>
      <anchor>aa37972e79cbc301db17f5b0b95964908</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::CommentNode</name>
    <filename>classxmlpp_1_1CommentNode.html</filename>
    <base>xmlpp::ContentNode</base>
    <member kind="function">
      <type></type>
      <name>CommentNode</name>
      <anchorfile>classxmlpp_1_1CommentNode.html</anchorfile>
      <anchor>a3be1e492187b87279acc1aff82c77dc2</anchor>
      <arglist>(_xmlNode *node)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~CommentNode</name>
      <anchorfile>classxmlpp_1_1CommentNode.html</anchorfile>
      <anchor>a69f6e389f1c683c86e3f5f6ff50cbf0d</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::ContentNode</name>
    <filename>classxmlpp_1_1ContentNode.html</filename>
    <base>xmlpp::Node</base>
    <member kind="function">
      <type></type>
      <name>ContentNode</name>
      <anchorfile>classxmlpp_1_1ContentNode.html</anchorfile>
      <anchor>af9fcf3459cdc338ffe51c09006487c3e</anchor>
      <arglist>(_xmlNode *node)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ContentNode</name>
      <anchorfile>classxmlpp_1_1ContentNode.html</anchorfile>
      <anchor>a695769ae0499d8e4831e0465f8e67838</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_content</name>
      <anchorfile>classxmlpp_1_1ContentNode.html</anchorfile>
      <anchor>add25e879109b4481281ccb876e684819</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_content</name>
      <anchorfile>classxmlpp_1_1ContentNode.html</anchorfile>
      <anchor>a2e9978caa82eb11cadccd4d5fc4e690a</anchor>
      <arglist>(const Glib::ustring &amp;content)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>is_white_space</name>
      <anchorfile>classxmlpp_1_1ContentNode.html</anchorfile>
      <anchor>ab032b4eff400d958ffbe57516bdb7f93</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::Element</name>
    <filename>classxmlpp_1_1Element.html</filename>
    <base>xmlpp::Node</base>
    <member kind="typedef">
      <type>std::list&lt; Attribute * &gt;</type>
      <name>AttributeList</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a7c54c83af3dfeddf2a70dba5ca6cc1d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Element</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>addce07f26b8ca52beb583d942375a756</anchor>
      <arglist>(_xmlNode *node)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Element</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a6633bfbb6b3b6520ab9ac8fa4991b066</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_namespace_declaration</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a801e28a85e4c45d145d056c5ff7186f3</anchor>
      <arglist>(const Glib::ustring &amp;ns_uri, const Glib::ustring &amp;ns_prefix=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>AttributeList</type>
      <name>get_attributes</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a8fbb85e7ecabc5a8b9b39638f94e4fb7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const AttributeList</type>
      <name>get_attributes</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>afc9883eee2a4fb0584a60b21b5d2673b</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Attribute *</type>
      <name>get_attribute</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a21fcf0bb1ee913fb698d2ab8911dda77</anchor>
      <arglist>(const Glib::ustring &amp;name, const Glib::ustring &amp;ns_prefix=Glib::ustring()) const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_attribute_value</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a8ae47935bca05ac2a6f0d8b02221a6b1</anchor>
      <arglist>(const Glib::ustring &amp;name, const Glib::ustring &amp;ns_prefix=Glib::ustring()) const </arglist>
    </member>
    <member kind="function">
      <type>Attribute *</type>
      <name>set_attribute</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a359b10f76db129fff5776c8405ac8e9a</anchor>
      <arglist>(const Glib::ustring &amp;name, const Glib::ustring &amp;value, const Glib::ustring &amp;ns_prefix=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remove_attribute</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a3d5765f07699dc8d4d55c0b0157defad</anchor>
      <arglist>(const Glib::ustring &amp;name, const Glib::ustring &amp;ns_prefix=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>TextNode *</type>
      <name>get_child_text</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a71885eacc99a1e7812b47d3d0607423a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const TextNode *</type>
      <name>get_child_text</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a0a2326904a79f2e2abb5c91a4107fa6f</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>TextNode *</type>
      <name>add_child_text</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a15cd56e871aaf9625b7f4fbedb97c764</anchor>
      <arglist>(const Glib::ustring &amp;content=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>TextNode *</type>
      <name>add_child_text</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>af774512e6c39e8e6741a0ff7bf9250f5</anchor>
      <arglist>(xmlpp::Node *previous_sibling, const Glib::ustring &amp;content=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>TextNode *</type>
      <name>add_child_text_before</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>aa05dee4b080f6e3c709397a48ad2a34e</anchor>
      <arglist>(xmlpp::Node *next_sibling, const Glib::ustring &amp;content=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_child_text</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a9be4234049fe13f2941d9c7a5dbc37e2</anchor>
      <arglist>(const Glib::ustring &amp;content)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>has_child_text</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a40d1299122bd9921fcb544ca1236aa95</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>CommentNode *</type>
      <name>add_child_comment</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>af69479710a4ab4cd8e89703a20dd360e</anchor>
      <arglist>(const Glib::ustring &amp;content)</arglist>
    </member>
    <member kind="function">
      <type>CdataNode *</type>
      <name>add_child_cdata</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a422cf7abd2dcc7da5a465f9b6afffd05</anchor>
      <arglist>(const Glib::ustring &amp;content)</arglist>
    </member>
    <member kind="function">
      <type>EntityReference *</type>
      <name>add_child_entity_reference</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a3a9aa8bc8745eb2521f705c1d468cc85</anchor>
      <arglist>(const Glib::ustring &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>ProcessingInstructionNode *</type>
      <name>add_child_processing_instruction</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>a1fec4ad7b2f01d71606432ab9dfc27dd</anchor>
      <arglist>(const Glib::ustring &amp;name, const Glib::ustring &amp;content)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>Glib::ustring</type>
      <name>get_namespace_uri_for_prefix</name>
      <anchorfile>classxmlpp_1_1Element.html</anchorfile>
      <anchor>af4c996b3eed332ed7e4e3096427e624d</anchor>
      <arglist>(const Glib::ustring &amp;ns_prefix) const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::EntityDeclaration</name>
    <filename>classxmlpp_1_1EntityDeclaration.html</filename>
    <base>xmlpp::ContentNode</base>
    <member kind="function">
      <type></type>
      <name>EntityDeclaration</name>
      <anchorfile>classxmlpp_1_1EntityDeclaration.html</anchorfile>
      <anchor>aa3d1fe2fc91b490ac48448e20d7689a8</anchor>
      <arglist>(_xmlNode *node)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~EntityDeclaration</name>
      <anchorfile>classxmlpp_1_1EntityDeclaration.html</anchorfile>
      <anchor>a7026727072e089d298f3645e63b88968</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_resolved_text</name>
      <anchorfile>classxmlpp_1_1EntityDeclaration.html</anchorfile>
      <anchor>a38859eff18b256eed9d6d5a7caa745e0</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_original_text</name>
      <anchorfile>classxmlpp_1_1EntityDeclaration.html</anchorfile>
      <anchor>a1390ad5ca69d9317a758c6fa3e0bfb52</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>_xmlEntity *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1EntityDeclaration.html</anchorfile>
      <anchor>af47060083f936f71ddf6e9e930b21555</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const _xmlEntity *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1EntityDeclaration.html</anchorfile>
      <anchor>ab6fbeb18deb5658d08028e644b81487a</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::EntityReference</name>
    <filename>classxmlpp_1_1EntityReference.html</filename>
    <base>xmlpp::Node</base>
    <member kind="function">
      <type></type>
      <name>EntityReference</name>
      <anchorfile>classxmlpp_1_1EntityReference.html</anchorfile>
      <anchor>a8f575183a2c1caa8e3a223f0292f83cf</anchor>
      <arglist>(_xmlNode *node)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~EntityReference</name>
      <anchorfile>classxmlpp_1_1EntityReference.html</anchorfile>
      <anchor>ac8dff5d5fe300000199be732311d6aff</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_resolved_text</name>
      <anchorfile>classxmlpp_1_1EntityReference.html</anchorfile>
      <anchor>aafe4e2f15ce6d04a2ec19e2f69cba19d</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_original_text</name>
      <anchorfile>classxmlpp_1_1EntityReference.html</anchorfile>
      <anchor>a49bda2ab52ea3db83f3c6e5eb8f2c467</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::Node</name>
    <filename>classxmlpp_1_1Node.html</filename>
    <base>xmlpp::NonCopyable</base>
    <member kind="typedef">
      <type>std::list&lt; Node * &gt;</type>
      <name>NodeList</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a7b0205c6df142d060dfca9119fadff49</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::map&lt; Glib::ustring, Glib::ustring &gt;</type>
      <name>PrefixNsMap</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a0175dca099d30f45ebbfd6a238420426</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Node</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a215d8c01b6b01596c4ea853f99dce8e7</anchor>
      <arglist>(_xmlNode *node)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Node</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a042c5de83dba6bf5d1923d76f1fb58c0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_name</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>ad8cba86cb7f43dd512f5b6817ed47d9a</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_name</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>ab5e1b1059a2af0818b1a8937fad49835</anchor>
      <arglist>(const Glib::ustring &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_namespace</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a4ddefaaaf3f0b550e776e9100bc23257</anchor>
      <arglist>(const Glib::ustring &amp;ns_prefix)</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_namespace_prefix</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a06790680c79808f1a56ddea438a6df5c</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_namespace_uri</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a9356e5ee02dbc809f9bf139b12befc6c</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>get_line</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a5e21f3a9996e25bd2df5ec8ce9906575</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>const Element *</type>
      <name>get_parent</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>aa8a68ac1887a33e44e3af365bd02db1b</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Element *</type>
      <name>get_parent</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a998e8cb924bd04abf72e57b68d2817f4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const Node *</type>
      <name>get_next_sibling</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a59a3e52fd752c243654a2cb58b1b0e18</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Node *</type>
      <name>get_next_sibling</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a2576ce35a6a3dd13b49e857524360ac4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const Node *</type>
      <name>get_previous_sibling</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>aa3ca83ba6e10ff21060be5d0441fa6ac</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Node *</type>
      <name>get_previous_sibling</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a567be4ebaffb2cd3dd2ae602c8df4a29</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>NodeList</type>
      <name>get_children</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>abb9d4d7aad234b9b01ae7fe404915a95</anchor>
      <arglist>(const Glib::ustring &amp;name=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>const NodeList</type>
      <name>get_children</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a2cb9f082c7bebc349c1f606e69788a57</anchor>
      <arglist>(const Glib::ustring &amp;name=Glib::ustring()) const </arglist>
    </member>
    <member kind="function">
      <type>Element *</type>
      <name>add_child</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>ac56c54f8006cbfb545642cbf2e96bc4a</anchor>
      <arglist>(const Glib::ustring &amp;name, const Glib::ustring &amp;ns_prefix=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>Element *</type>
      <name>add_child</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a57bc6d6c941f1477a2445609d49ce645</anchor>
      <arglist>(xmlpp::Node *previous_sibling, const Glib::ustring &amp;name, const Glib::ustring &amp;ns_prefix=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>Element *</type>
      <name>add_child_before</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a9dff93a466cfbeda0357dfc9a3bd316c</anchor>
      <arglist>(xmlpp::Node *next_sibling, const Glib::ustring &amp;name, const Glib::ustring &amp;ns_prefix=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remove_child</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a00ccfb39d9b6d82b5acd5f458c8f8ca8</anchor>
      <arglist>(Node *node)</arglist>
    </member>
    <member kind="function">
      <type>Node *</type>
      <name>import_node</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>aea42eae72fa7901aa1fb8f5c0fd194ec</anchor>
      <arglist>(const Node *node, bool recursive=true)</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_path</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>ad05b43e18a4600ada423b74a59efb541</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>NodeSet</type>
      <name>find</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a25e56c6ae44be0926d1efcc7e488d96f</anchor>
      <arglist>(const Glib::ustring &amp;xpath) const </arglist>
    </member>
    <member kind="function">
      <type>NodeSet</type>
      <name>find</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a4bfbd6ed354524c960d6cd425c534c7c</anchor>
      <arglist>(const Glib::ustring &amp;xpath, const PrefixNsMap &amp;namespaces) const </arglist>
    </member>
    <member kind="function">
      <type>_xmlNode *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>aeede250f5cf6de5f55cb7bb94084ec7c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const _xmlNode *</type>
      <name>cobj</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a831eea0dbe1e51a350a9b20a2da47fc9</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>create_wrapper</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a616a872d0fbd86206c0beee0be5abee3</anchor>
      <arglist>(_xmlNode *node)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>free_wrappers</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>a9e92d80344958a33f8e32ade0bd18fa5</anchor>
      <arglist>(_xmlNode *attr)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>_xmlNode *</type>
      <name>create_new_child_node</name>
      <anchorfile>classxmlpp_1_1Node.html</anchorfile>
      <anchor>af8d31aaa506d229651e8328f8294f5df</anchor>
      <arglist>(const Glib::ustring &amp;name, const Glib::ustring &amp;ns_prefix)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::ProcessingInstructionNode</name>
    <filename>classxmlpp_1_1ProcessingInstructionNode.html</filename>
    <base>xmlpp::ContentNode</base>
    <member kind="function">
      <type></type>
      <name>ProcessingInstructionNode</name>
      <anchorfile>classxmlpp_1_1ProcessingInstructionNode.html</anchorfile>
      <anchor>a609246dff62dbc2bc86eaf9a8c393fda</anchor>
      <arglist>(_xmlNode *node)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ProcessingInstructionNode</name>
      <anchorfile>classxmlpp_1_1ProcessingInstructionNode.html</anchorfile>
      <anchor>a34195051d18ef6ec70f8b6f01ab9e74d</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::TextNode</name>
    <filename>classxmlpp_1_1TextNode.html</filename>
    <base>xmlpp::ContentNode</base>
    <member kind="function">
      <type></type>
      <name>TextNode</name>
      <anchorfile>classxmlpp_1_1TextNode.html</anchorfile>
      <anchor>a4838cc566ae92f31c3773ce8b03b061d</anchor>
      <arglist>(_xmlNode *node)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~TextNode</name>
      <anchorfile>classxmlpp_1_1TextNode.html</anchorfile>
      <anchor>ab36c29b54db0cd5e910a8cbc89d35bd2</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::Parser</name>
    <filename>classxmlpp_1_1Parser.html</filename>
    <base protection="private">xmlpp::NonCopyable</base>
    <member kind="typedef">
      <type>unsigned int</type>
      <name>size_type</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a8e7c797006bb5398629cf98f4141e3e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Parser</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>ab2990f22147cb2163eda6e773fb2eb68</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Parser</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>acf64cbff6b9cf4010226fc1c803d56f9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>set_validate</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a39332721610231ca8c5114e245121cc1</anchor>
      <arglist>(bool val=true)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>get_validate</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a7329d0daf70030bef41b92e6ed22bd63</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>set_substitute_entities</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a960e9ee12df9b631c694ca002932cf7f</anchor>
      <arglist>(bool val=true)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>get_substitute_entities</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>ae71762868b009ccae8f00c1d34df00e7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_throw_messages</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a3d491c0479b11e5766849956cd4b4538</anchor>
      <arglist>(bool val=true)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>get_throw_messages</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>ad9e5523ffa19aa3e76761a0d46040efb</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>parse_file</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>ad0855eff9a35a9160800ad58ad19fd7e</anchor>
      <arglist>(const Glib::ustring &amp;filename)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>parse_memory</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>abbbf646035570f1aa5366558dfc2ae01</anchor>
      <arglist>(const Glib::ustring &amp;contents)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>parse_stream</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>ad7134b1c68308351785acb0302aa54db</anchor>
      <arglist>(std::istream &amp;in)=0</arglist>
    </member>
    <member kind="enumeration">
      <name>MsgType</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a8bfd5ceb1cc0b3b9fd9ff7cf44d1029b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MsgParserError</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a8bfd5ceb1cc0b3b9fd9ff7cf44d1029bae3a15565bbac2bbce269c71578874290</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MsgParserWarning</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a8bfd5ceb1cc0b3b9fd9ff7cf44d1029ba2699545b0c4313c08db938000a1ff385</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MsgValidityError</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a8bfd5ceb1cc0b3b9fd9ff7cf44d1029bab5b788bc9b710543d1061be8e8d1ceff</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MsgValidityWarning</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a8bfd5ceb1cc0b3b9fd9ff7cf44d1029baf56ce30abc58b13a1bd9ee27c085ac70</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>initialize_context</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a25f92d0a820d96accfe316a066bade49</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>release_underlying</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a346ebfb1219e1c0d3acbd017a07706e6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_validity_error</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a36860900bd7f118adfd7b7887278e4fb</anchor>
      <arglist>(const Glib::ustring &amp;message)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_validity_warning</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>ad0876425a0d687f24bc54b44d950ab2c</anchor>
      <arglist>(const Glib::ustring &amp;message)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>handleException</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a860e1cff3da52a4bbb0b17a06475399f</anchor>
      <arglist>(const exception &amp;e)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>check_for_exception</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a9daba11c6746d91153329a9dbd8decc5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>check_for_validity_messages</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>aad45390ab3d30d1dea26001f2d6a9a2f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>callback_parser_error</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>ae5272535b144daaafcdc161e36f1aa33</anchor>
      <arglist>(void *ctx, const char *msg,...)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>callback_parser_warning</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a7dbe69dcd8aa5c7625fa96a9dcdcbad7</anchor>
      <arglist>(void *ctx, const char *msg,...)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>callback_validity_error</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a7ec91483923a6023e7ac56eeaf16b1e3</anchor>
      <arglist>(void *ctx, const char *msg,...)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>callback_validity_warning</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>aa573fc538688db238714cef413944567</anchor>
      <arglist>(void *ctx, const char *msg,...)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>callback_error_or_warning</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a49b79a6ada464e8d1be27a655ee63b48</anchor>
      <arglist>(MsgType msg_type, void *ctx, const char *msg, va_list var_args)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>_xmlParserCtxt *</type>
      <name>context_</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a2b49a51f1a8fce897dffcb1e81f87907</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>exception *</type>
      <name>exception_</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a7b2a100f91d2ead57de5ef640ebf91a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Glib::ustring</type>
      <name>validate_error_</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a367b4132d663f1c1adf1515c4a6e959f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Glib::ustring</type>
      <name>validate_warning_</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>aac64f7a377ae85c848dbb8008a43f555</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>validate_</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a2d21ddd4fdd34f845937649eb4fb8814</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>substitute_entities_</name>
      <anchorfile>classxmlpp_1_1Parser.html</anchorfile>
      <anchor>a969bc262133a8310cfbfc097f925c00c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::SaxParser</name>
    <filename>classxmlpp_1_1SaxParser.html</filename>
    <base>xmlpp::Parser</base>
    <class kind="struct">xmlpp::SaxParser::Attribute</class>
    <class kind="struct">xmlpp::SaxParser::AttributeHasName</class>
    <member kind="typedef">
      <type>std::deque&lt; Attribute &gt;</type>
      <name>AttributeList</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>a1cb4e32dd3adf2460f2836bddb59633c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SaxParser</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>a66bfaafc79c00197010f8d22b2a39df8</anchor>
      <arglist>(bool use_get_entity=false)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~SaxParser</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>a3f69d5fe3472240225dd46f8c70a4e79</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_file</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>ac2b0cf0069a73fa92c80f6d455923698</anchor>
      <arglist>(const Glib::ustring &amp;filename)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_memory</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>ab9367c59f02e0db5a2b17cd48ddf74f9</anchor>
      <arglist>(const Glib::ustring &amp;contents)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>parse_memory_raw</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>aff9643a2764f6d8caee0307abbfcca25</anchor>
      <arglist>(const unsigned char *contents, size_type bytes_count)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_stream</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>aaa731e56f99aa7f8313a1f096389f723</anchor>
      <arglist>(std::istream &amp;in)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_chunk</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>a24b4440b0539a0015890182b35396b5d</anchor>
      <arglist>(const Glib::ustring &amp;chunk)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>parse_chunk_raw</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>a501a19205773ac1c843d6007b189860e</anchor>
      <arglist>(const unsigned char *contents, size_type bytes_count)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>finish_chunk_parsing</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>ac540859149b064cfce87931ba736f3c5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_start_document</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>a0a33932c84e245284414003f668b829f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_end_document</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>aa7b3090025892829af91f8c51a06ef7b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_start_element</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>a1496022dd6b58d9af6a4a7a703830a84</anchor>
      <arglist>(const Glib::ustring &amp;name, const AttributeList &amp;attributes)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_end_element</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>a5c1c00b47c020e3cd8d623361c6909fa</anchor>
      <arglist>(const Glib::ustring &amp;name)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_characters</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>a359b48ea1c15cb39837f680a1f2fb2c2</anchor>
      <arglist>(const Glib::ustring &amp;characters)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_comment</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>acccf558fc47549bdad076201193739f3</anchor>
      <arglist>(const Glib::ustring &amp;text)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_warning</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>ab951160501bf3e78901691a79e3d11e8</anchor>
      <arglist>(const Glib::ustring &amp;text)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_error</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>a38c3c5f9bad26030acee20d7ddf4936a</anchor>
      <arglist>(const Glib::ustring &amp;text)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_fatal_error</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>ad951cc2b1fe314805c5d735a0d7f467c</anchor>
      <arglist>(const Glib::ustring &amp;text)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_cdata_block</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>af6671ab7dbda78730a65dcbb58aa0ec1</anchor>
      <arglist>(const Glib::ustring &amp;text)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_internal_subset</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>a4b453817ac37ea307c2ba023cc938794</anchor>
      <arglist>(const Glib::ustring &amp;name, const Glib::ustring &amp;publicId, const Glib::ustring &amp;systemId)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual _xmlEntity *</type>
      <name>on_get_entity</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>aeb20c4424f325655ebd787c2b2268137</anchor>
      <arglist>(const Glib::ustring &amp;name)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_entity_declaration</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>ab0985199cbdbd9a7ab31b024acf3c2c8</anchor>
      <arglist>(const Glib::ustring &amp;name, XmlEntityType type, const Glib::ustring &amp;publicId, const Glib::ustring &amp;systemId, const Glib::ustring &amp;content)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>release_underlying</name>
      <anchorfile>classxmlpp_1_1SaxParser.html</anchorfile>
      <anchor>a839554028d8bad81774040eb67d23103</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>xmlpp::SaxParser::Attribute</name>
    <filename>structxmlpp_1_1SaxParser_1_1Attribute.html</filename>
    <member kind="function">
      <type></type>
      <name>Attribute</name>
      <anchorfile>structxmlpp_1_1SaxParser_1_1Attribute.html</anchorfile>
      <anchor>af97625fc6ae95e07aff714558c6ff3a5</anchor>
      <arglist>(Glib::ustring const &amp;n, Glib::ustring const &amp;v)</arglist>
    </member>
    <member kind="variable">
      <type>Glib::ustring</type>
      <name>name</name>
      <anchorfile>structxmlpp_1_1SaxParser_1_1Attribute.html</anchorfile>
      <anchor>ac05c407cd28c6115ae860cb12f12ed2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Glib::ustring</type>
      <name>value</name>
      <anchorfile>structxmlpp_1_1SaxParser_1_1Attribute.html</anchorfile>
      <anchor>abcd5e062b0e9fe921e1ce014fd71bbc8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>xmlpp::SaxParser::AttributeHasName</name>
    <filename>structxmlpp_1_1SaxParser_1_1AttributeHasName.html</filename>
    <member kind="function">
      <type></type>
      <name>AttributeHasName</name>
      <anchorfile>structxmlpp_1_1SaxParser_1_1AttributeHasName.html</anchorfile>
      <anchor>aaefc0da59addd685474657ecf4c88899</anchor>
      <arglist>(Glib::ustring const &amp;n)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>structxmlpp_1_1SaxParser_1_1AttributeHasName.html</anchorfile>
      <anchor>afbd0f609949146293958662da45f75fc</anchor>
      <arglist>(Attribute const &amp;attribute)</arglist>
    </member>
    <member kind="variable">
      <type>Glib::ustring const &amp;</type>
      <name>name</name>
      <anchorfile>structxmlpp_1_1SaxParser_1_1AttributeHasName.html</anchorfile>
      <anchor>a3a12fda2735ef1a271a8433f1f1572b8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::DomParser</name>
    <filename>classxmlpp_1_1DomParser.html</filename>
    <base>xmlpp::Parser</base>
    <member kind="function">
      <type></type>
      <name>DomParser</name>
      <anchorfile>classxmlpp_1_1DomParser.html</anchorfile>
      <anchor>abeab430d4863459b6d03fc7e97363df5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DomParser</name>
      <anchorfile>classxmlpp_1_1DomParser.html</anchorfile>
      <anchor>a9538baed354098e8ac73f69e1d4f55e5</anchor>
      <arglist>(const Glib::ustring &amp;filename, bool validate=false)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~DomParser</name>
      <anchorfile>classxmlpp_1_1DomParser.html</anchorfile>
      <anchor>a3a18958b3980fb3ff44441868da7cb6a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_file</name>
      <anchorfile>classxmlpp_1_1DomParser.html</anchorfile>
      <anchor>a437c91bb64033669649d91f28ecc7dc9</anchor>
      <arglist>(const Glib::ustring &amp;filename)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_memory</name>
      <anchorfile>classxmlpp_1_1DomParser.html</anchorfile>
      <anchor>a2110398d5a19320e7bbe65e0edb2ab9c</anchor>
      <arglist>(const Glib::ustring &amp;contents)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>parse_memory_raw</name>
      <anchorfile>classxmlpp_1_1DomParser.html</anchorfile>
      <anchor>aae7bd491f8aae685900bec03ec43d855</anchor>
      <arglist>(const unsigned char *contents, size_type bytes_count)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_stream</name>
      <anchorfile>classxmlpp_1_1DomParser.html</anchorfile>
      <anchor>a3d02d6c5b4194b47118e92a8e258743c</anchor>
      <arglist>(std::istream &amp;in)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator bool</name>
      <anchorfile>classxmlpp_1_1DomParser.html</anchorfile>
      <anchor>af6455076d91fc65e39f82e9a6c7ad764</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Document *</type>
      <name>get_document</name>
      <anchorfile>classxmlpp_1_1DomParser.html</anchorfile>
      <anchor>ad10907d495f9e1e2b63638a9c69f78a8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const Document *</type>
      <name>get_document</name>
      <anchorfile>classxmlpp_1_1DomParser.html</anchorfile>
      <anchor>a4a3452e46cf656998d8752ed949239de</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_context</name>
      <anchorfile>classxmlpp_1_1DomParser.html</anchorfile>
      <anchor>a0f77c8ab0a9ab2d7abd809846ae75d6f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>release_underlying</name>
      <anchorfile>classxmlpp_1_1DomParser.html</anchorfile>
      <anchor>a58a312db2834a41a77ca290dc6dc48fe</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Document *</type>
      <name>doc_</name>
      <anchorfile>classxmlpp_1_1DomParser.html</anchorfile>
      <anchor>ab95c1209f2311df8fe1f1f34651d2d28</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::TextReader</name>
    <filename>classxmlpp_1_1TextReader.html</filename>
    <base protection="private">xmlpp::NonCopyable</base>
    <member kind="enumeration">
      <name>xmlNodeType</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Attribute</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40ea953f5d2eee7b77bb88bf11cdef6c9f0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CDATA</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40eaa6111071444413bbe7a3d703adaa4428</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Comment</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40ea3bb150c1fe40950967663e14ac4eee21</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Document</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40eae3efc570dfef9cc2b59008ac419d9b2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DocumentFragment</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40ea31a65d34484b940b1fe5005383db2ab1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DocumentType</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40ea89be2703d2112a83df749542970bf220</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Element</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40ea196fbf9ee1b62edf2fd6e4aeaa5b8a09</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EndElement</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40ea10ded28eb805871af4aa96ff1366360d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EndEntity</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40eab1bb7cadf025bc34b5e8aeb645f0d3e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Entity</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40ea025e694e532d3ef51ee336d6f43db7b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EntityReference</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40ea91562173c93bfbf1d08f2bb0084c0fe6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>None</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40eae7ab9017681e2d0ab1cf6c1b133b81a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Notation</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40ea4eb093e0c99c5652f82ad727e8a4b814</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ProcessingInstruction</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40ea32b2b75764fe7e2637a5b22a128ef7e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SignificantWhitespace</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40eae1189bd50d5f0a6324db79310b5e7896</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Text</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40ea3e9d8486bba549a19e00964c2255d879</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Whitespace</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40eaac0a2cbaedfcb9a80f66eea04ccc0a54</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>XmlDeclaration</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9057500ac7b1972ff2371794ba48f40ea9a18f29beb9bb8e3aeba11c2b031d314</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>xmlReadState</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a87dca472c41f1554155567352caae8b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Closed</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a87dca472c41f1554155567352caae8b1abbce86084aed205faaacc744bd0357e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>EndOfFile</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a87dca472c41f1554155567352caae8b1afdc5accdafe700a08de1c7a996a8daad</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Error</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a87dca472c41f1554155567352caae8b1a796c014e6177dafba41a3318c5b52e53</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Initial</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a87dca472c41f1554155567352caae8b1ab21a7b6e1f6a04f07857d468aacfcb00</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Interactive</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a87dca472c41f1554155567352caae8b1a2f35a806ccea1c7e50578c98e740b053</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Reading</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a87dca472c41f1554155567352caae8b1a6c870210b11f64532e0ee73ac5c6dac7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>ParserProperties</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a5cb6d36f5367eec5e986d815e60ad292</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LoadDtd</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a5cb6d36f5367eec5e986d815e60ad292aedcccbfdca3500eca880ac102090cf04</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DefaultAttrs</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a5cb6d36f5367eec5e986d815e60ad292ae7ff80120d6a00927f44cf9f9d3e01af</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Validate</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a5cb6d36f5367eec5e986d815e60ad292ac293cac644cdebce4b90b45a501ee356</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SubstEntities</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a5cb6d36f5367eec5e986d815e60ad292ab79c0aee880cdac3a1028d7464e13b01</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned int</type>
      <name>size_type</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a659b255f7221dbbae3e98081bbfc6f19</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>TextReader</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>ae40f56d8714593bef690bedafad23f8f</anchor>
      <arglist>(struct _xmlTextReader *cobj)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>TextReader</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a710cf31f25d144bb790180c53acda367</anchor>
      <arglist>(const Glib::ustring &amp;URI)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>TextReader</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>ac5cdb5b6d5c8e6809d65a398f417a1fb</anchor>
      <arglist>(const unsigned char *data, size_type size, const Glib::ustring &amp;uri=Glib::ustring())</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~TextReader</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a35043a6525290428f3724e1a31918bb8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>read</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a3a3ee347087889b3f3ef5463dd777ad8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>read_inner_xml</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a348c936b9ed75c15456ddea2eea80eb7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>read_outer_xml</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a5a0e5b3823151de60faf552c4709daf9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>read_string</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a9110986142ac718978d8993e26b00c85</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>read_attribute_value</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a109934670bab6176dc1a8132f1afd5d0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>get_attribute_count</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a02e2868ecf9710edab57f292c92590d0</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_base_uri</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>acab85f9c6c50a51919428e545ef3e9cf</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>get_depth</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a6b42a09464b9817813baf4f8daae4974</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>has_attributes</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a1c59719f3e914090405b8b8fbeef2487</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>has_value</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a16cd0ff7a5db226f388ee1ca9bf7f9e8</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>is_default</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>ad9a86890436d9c16536e271914148033</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>is_empty_element</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a77ca814cf125c28df7c1703974596b0b</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_local_name</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a0d54b789484bd7502a16119a1ec5a83b</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_name</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a76e93fe81196de902ff3f3f9ea35ecbe</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_namespace_uri</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a708df8f8d3d3d7240547d4d8c940cccb</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>xmlNodeType</type>
      <name>get_node_type</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>aa9146ad2bd6547ac1b9dad007867d70d</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_prefix</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a0478cbc006341af4d0f5f356d6e002bd</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>get_quote_char</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a33fee208ed6c2ad303b39c7b41abfa59</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_value</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a8cb6e4bec1e0873d2a9a2b385383c505</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_xml_lang</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a1e94e9c3eb8e0fa0c3ff4d2c8f182480</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>xmlReadState</type>
      <name>get_read_state</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a71ae70e90b1a118517038ec194bfcddd</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>close</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a5cf337c94758e54214f7bbe23f61b13d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_attribute</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a02a5471219d98c52f5c77a10905a8bac</anchor>
      <arglist>(int number) const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_attribute</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a0988d72f2cbbb8e426df972cb5363cd3</anchor>
      <arglist>(const Glib::ustring &amp;name) const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>get_attribute</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a07853abaa889c5a34dc847625c377ac1</anchor>
      <arglist>(const Glib::ustring &amp;local_name, const Glib::ustring &amp;ns_uri) const </arglist>
    </member>
    <member kind="function">
      <type>Glib::ustring</type>
      <name>lookup_namespace</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>adb240997e9387b0fd8fb02c68da4fcf3</anchor>
      <arglist>(const Glib::ustring &amp;prefix) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>move_to_attribute</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a78ef028f9250afaca44928847418153a</anchor>
      <arglist>(int number)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>move_to_attribute</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a57c157282664d34ec16374aa59c973b6</anchor>
      <arglist>(const Glib::ustring &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>move_to_attribute</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a6a8b5834af3d6254b96ba6b4453b98ce</anchor>
      <arglist>(const Glib::ustring &amp;local_name, const Glib::ustring &amp;ns_uri)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>move_to_first_attribute</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a724cd1e0238c14c2d2b797b555392fb1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>move_to_next_attribute</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a3ffee74ac337f73f3205d23c3c0eafec</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>move_to_element</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>ae1164a49d5dc855bdc110b0826f752d7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>get_normalization</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a37439f2b2a4ba91ad59ffc5a18713e85</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_normalization</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>aaf2974eef9c1c77abf50e9c8584f9c40</anchor>
      <arglist>(bool value)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>get_parser_property</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a23c7ad0edbbdff15c393d485653b9cd5</anchor>
      <arglist>(ParserProperties property) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_parser_property</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a8e5db1b68c5227bf748dfee7f36e8074</anchor>
      <arglist>(ParserProperties property, bool value)</arglist>
    </member>
    <member kind="function">
      <type>Node *</type>
      <name>get_current_node</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a830def19dd26d2c3fb131302f8ed1a31</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const Node *</type>
      <name>get_current_node</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a51a5a390a1824f281f46fdcd00e212c2</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Node *</type>
      <name>expand</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a6eeec8656e65a93c7b7496f17d288882</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>next</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>a03c7204c20277f446c64cc7ca15815b2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>is_valid</name>
      <anchorfile>classxmlpp_1_1TextReader.html</anchorfile>
      <anchor>aa0eab6a0a001fe68d90584688c24cc81</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::DtdValidator</name>
    <filename>classxmlpp_1_1DtdValidator.html</filename>
    <base>xmlpp::Validator</base>
    <member kind="function">
      <type></type>
      <name>DtdValidator</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>aa14c1fdd08db52e64ec53d7ea19f63e2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DtdValidator</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>afda71639e4241bd7c96de31a3e341e48</anchor>
      <arglist>(const Glib::ustring &amp;file)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DtdValidator</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>a376f883bc1455188861455727f2d3039</anchor>
      <arglist>(const Glib::ustring &amp;external, const Glib::ustring &amp;system)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~DtdValidator</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>af721781854a6ae3312ab2768f42ecca6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_subset</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>ac4fb98a9ddb4d6d7afa8aa933ec2757c</anchor>
      <arglist>(const Glib::ustring &amp;external, const Glib::ustring &amp;system)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_file</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>a0b3e4347421d7dd6fa0ff6b53b5c95dc</anchor>
      <arglist>(const Glib::ustring &amp;filename)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_memory</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>ab71df31e371de03e9d0403be51ac483a</anchor>
      <arglist>(const Glib::ustring &amp;contents)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_stream</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>aa9e6b29e5bcd9e154c8291209b461595</anchor>
      <arglist>(std::istream &amp;in)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator bool</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>a59a9cc61c0dafc9768995d5f807eeb2d</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Dtd *</type>
      <name>get_dtd</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>a51385000784f05b4052ec6846a7689ad</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const Dtd *</type>
      <name>get_dtd</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>aaf69006f668f0c8f960e9ede125e2b86</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validate</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>a45d51d91fecdfcd659f4bd2008d1d686</anchor>
      <arglist>(const Document *doc)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>release_underlying</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>ae132f171961e6fa5d3745a03f8d9a210</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Dtd *</type>
      <name>dtd_</name>
      <anchorfile>classxmlpp_1_1DtdValidator.html</anchorfile>
      <anchor>a31fdef5610cfc931496428013a8858a8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::SchemaValidator</name>
    <filename>classxmlpp_1_1SchemaValidator.html</filename>
    <base>xmlpp::Validator</base>
    <member kind="function">
      <type></type>
      <name>SchemaValidator</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>a9afc7540342e5a42c3ef5a070bac24c1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SchemaValidator</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>aec08104bcf0c65636c86ef97b8b7fcf0</anchor>
      <arglist>(const Glib::ustring &amp;file)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SchemaValidator</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>aac78482c8469bd1a01837f5c00b3b998</anchor>
      <arglist>(Document &amp;document)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SchemaValidator</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>aa58e4c2ccdd7505679abb40b7933dc11</anchor>
      <arglist>(Schema *schema)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~SchemaValidator</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>a7fe4caea42d2990c6a2a1edc98b01b79</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_file</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>a98a4e62c6ab0c529370ebce44aef98b6</anchor>
      <arglist>(const Glib::ustring &amp;filename)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_memory</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>af7131145e3c4b81dff4b32a23b9a5329</anchor>
      <arglist>(const Glib::ustring &amp;contents)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>parse_document</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>a3924e29fad84f2f24a3d3865e19a7fbd</anchor>
      <arglist>(Document &amp;document)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>set_schema</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>a659de07c9da53a1f883544778b28d117</anchor>
      <arglist>(Schema *schema)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator bool</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>a90bfbb25fb29adc1b77d40c393f7bc41</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Schema *</type>
      <name>get_schema</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>a0a6acd87370c89eea0dfb343592efe0b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const Schema *</type>
      <name>get_schema</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>afe5fcf23c3c8f89eeb225ac0b416e545</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validate</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>a2f78aaf2cee2230d0773d6caf52d8134</anchor>
      <arglist>(const Document *doc)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validate</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>ade440860cfcbbb0ab082bf42dd03d113</anchor>
      <arglist>(const Glib::ustring &amp;file)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>initialize_valid</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>ad651b57c399b15ba536371e287001cc1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>parse_context</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>ac730471f843fb9eda4fee1f499b30271</anchor>
      <arglist>(_xmlSchemaParserCtxt *context)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>release_underlying</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>a682a1a79512bc7bc58c839212f2a2722</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Schema *</type>
      <name>schema_</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>a4f65ec72cd052a13df2da5dfd8004e41</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>embbeded_shema_</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>ac5810e47f11fe9a4cdf5895c209fdcb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>_xmlSchemaValidCtxt *</type>
      <name>ctxt_</name>
      <anchorfile>classxmlpp_1_1SchemaValidator.html</anchorfile>
      <anchor>a484c2d1670616ff07d31034f76850e2a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>xmlpp::Validator</name>
    <filename>classxmlpp_1_1Validator.html</filename>
    <base protection="private">xmlpp::NonCopyable</base>
    <member kind="function">
      <type></type>
      <name>Validator</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>a2e25d9106542fac59c7a4368257cdcb5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Validator</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>a381ade9a196777fb24780375ed82b06a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>initialize_valid</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>a12ee744e385987547d1c449a1e4bf596</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>release_underlying</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>a317466ca90b8e53d6f57e49fd032d8cb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_validity_error</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>af59b45514fe397db1dbb025a1a3b103f</anchor>
      <arglist>(const Glib::ustring &amp;message)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>on_validity_warning</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>a7ef36a34b258165082c4f33737e743d2</anchor>
      <arglist>(const Glib::ustring &amp;message)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>handleException</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>a4d02cc2d687f0bd28e3c777ebbc7c458</anchor>
      <arglist>(const exception &amp;e)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>check_for_exception</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>a2af695894f22e61b4f85551edefd104d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>check_for_validity_messages</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>a655f06aa683577c26dcb2edba2e9e84b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>callback_validity_error</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>a616cad28f3d4f59b76edb916bffeb6ef</anchor>
      <arglist>(void *ctx, const char *msg,...)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>callback_validity_warning</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>a5f8938e79ca2c52ec26e499b356985a0</anchor>
      <arglist>(void *ctx, const char *msg,...)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>_xmlValidCtxt *</type>
      <name>valid_</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>a1c6ef225bbdf9053a7e83587262f30e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>exception *</type>
      <name>exception_</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>a55796f9903e66aa4730313936fab1f0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Glib::ustring</type>
      <name>validate_error_</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>ac28c8c06c9c3cbbebfa0d1c90d20cdf5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Glib::ustring</type>
      <name>validate_warning_</name>
      <anchorfile>classxmlpp_1_1Validator.html</anchorfile>
      <anchor>aa876f0e05457de97a3c2d598e4af2580</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
