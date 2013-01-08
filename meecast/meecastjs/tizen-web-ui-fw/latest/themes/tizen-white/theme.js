(function( $, undefined ) {
//$.mobile.page.prototype.options.backBtnTheme	= "s";

// Clear default theme for child elements
$.mobile.page.prototype.options.headerTheme		= "s";
$.mobile.page.prototype.options.footerTheme		= "s";
//$.mobile.page.prototype.options.contentTheme	= "s";

// clear listview
$.mobile.listview.prototype.options.theme = "s";
$.mobile.listview.prototype.options.countTheme = "s";
$.mobile.listview.prototype.options.headerTheme = "s";
$.mobile.listview.prototype.options.dividerTheme = "s";
$.mobile.listview.prototype.options.splitTheme = "s";

//clear button theme
$.mobile.button.prototype.options.theme = "s";
$.fn.buttonMarkup.defaults.theme = "s";

// Default theme swatch
$.mobile.page.prototype.options.theme = "s";

// Default font size for this theme
$.tizen.frameworkData.defaultFontSize = 36;

})(jQuery);
