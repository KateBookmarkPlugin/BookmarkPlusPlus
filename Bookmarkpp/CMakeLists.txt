find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include_directories(${KDE4_INCLUDES})
 
# We are calling our plugin "ktexteditor_timedate", and it contains only
# one source file: timedate.cpp.
kde4_add_plugin(bookmarkpp bookmarkpp.cpp)
 
# We need to link our plugin against kdecore libs, as well as ktexteditor
target_link_libraries(bookmarkpp ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} ktexteditor)
 
# Well, we want to install our plugin on the plugin directory
install(TARGETS bookmarkpp DESTINATION ${PLUGIN_INSTALL_DIR})
 
# We also want to install the resource contents file on the data directory, at
# the subdirectory of our plugin name, so it does not mix up with other resource
# contents files.
install(FILES ui.rc DESTINATION ${DATA_INSTALL_DIR}/bookmarkpp)
 
# We want to install the desktop file that describes our plugin too. It will go
# on the services directory.
install(FILES bookmarkpp.desktop DESTINATION ${SERVICES_INSTALL_DIR})