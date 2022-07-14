idf_component_register(SRCS 
			"main.cpp" 
			"../lib/network_mqtt.c"
			"../lib/network_wifi.c"
			"../lib/libattopng.c"
			"../lib/vibe-background-sequential.c"
			"../lib/detect_api.c"
			"../lib/detect_functions.c"
			"../lib/feature_extraction.c"
			"../lib/helper.c"
			"../lib/erosionByAnchor.c"
			"../lib/sntp_controller.c"
			"../cpplib/human_object.cpp"
			"../cpplib/tracking.cpp"

             INCLUDE_DIRS "."
			"../lib"
			"../include"
			"../cpplib"
			"../cppinclude")

