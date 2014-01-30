##############################
#
# WELCOME
#
# This is the main cmake configuration file
# associated with your project
# You can define specific compilation options here
#
# More about cmake : www.cmake.org


############################ PROJECT NAME
PROJECT( Simulation )
 SET(QT_USE_QTNETWORK 1)
 SET(QT_USE_QTMAIN 1)
INCLUDE( ${QT_USE_FILE} ${QT_USE_QTNETWORK} )

############################ DEFINITION
# Add here definition needed.
ADD_DEFINITIONS( ${ALCOMMON_DEFINITIONS} ${PTHREAD_DEFINITIONS} ${TINYXML_DEFINITIONS} ${BOOST_DEFINITIONS}${QT_DEFINITIONS})

############################ INCLUDE DIRECTORY
# Define include directories
INCLUDE_DIRECTORIES( ${PROXIES_INCLUDE_DIR} ${ALCOMMON_INCLUDE_DIR} ${PTHREAD_INCLUDE_DIR}  ${TINYXML_INCLUDE_DIR} ${BOOST_INCLUDE_DIR}${QT_INCLUDE_DIR} )





############################ PROJECT SOURCES FILES
# Add here source files needed to compile this project
SET(PROJECT_SRCS
	../Module/Core/Thread.h  ../Module/Core/Thread.cpp ../Module/Core/MessageQueue.h  ../Module/Core/MessageQueue.cpp

	../Module/Representations/Blackboard.cpp  ../Module/Representations/Blackboard.h  ../Module/Representations/KS.cpp  
	../Module/Representations/KS.h  ../Module/Representations/KsComm.h ../Module/Representations/KSManager.cpp  
	../Module/Representations/KSManager.h  ../Module/Representations/BaseProvider.h
	../Module/Representations/Perception/VisionHeader.h
	../Module/Representations/Perception/VisionInterface.cpp
	../Module/Representations/Perception/VisionInterface.h

	../Module/Common/CommandMsg.h  ../Module/Common/CommandMsg.cpp
	../Module/Common/Geometry.cpp  ../Module/Common/Geometry.h
	../Module/Common/RingBuffer.h

	../Module/Vision/BallSpecialist.cpp  ../Module/Vision/BallSpecialist.h  
	../Module/Vision/BresenhamLineScan.cpp  ../Module/Vision/BresenhamLineScan.h ../Module/Vision/DebugDrawings.h  
	../Module/Vision/GoalSpecialist.h ../Module/Vision/GoalSpecialist.cpp
	../Module/Vision/Run.h  ../Module/Vision/ScanVision.h  ../Module/Vision/ScanVision.cpp ../Module/Vision/SensorComm.h  
	../Module/Vision/SensorComm.cpp
	../Module/Vision/LineFinder.h  ../Module/Vision/LineFinder.cpp  ../Module/Vision/TJImage.h  ../Module/Vision/TJImage.cpp

	../Module/Network/networkComm/wdatainfo.h  
	../Module/Network/networkComm/wdatainfo.cpp

	../Tools/Math/Pose2D.h  ../Tools/Math/Pose3D.h  ../Tools/Math/Range.h ../Tools/Math/Vector2.h  ../Tools/Math/Vector3.h 
	../Tools/Math/Common.h

	network/ConnectDialog.cpp   network/ConnectDialog.h  network/ConnectInfo.cpp  network/ConnectInfo.h
	network/NetCommandMsg.cpp  network/NetCommandMsg.h 
	network/ProduceData.h   network/ProduceData.cpp  network/receivethread.h  network/receivethread.cpp 
	network/sendthread.cpp  network/sendthread.h
	network/Tcpconfig.h network/TcpConnect.cpp network/TcpConnect.h  

	NaoProvider/SimuNaoProvider.cpp  NaoProvider/SimuNaoProvider.h
	NaoProvider/VisionSimulation.cpp  NaoProvider/VisionSimulation.h

	VisionGUI/GLOABALTODRAW.H  VisionGUI/GLOABLTODRAW.cpp VisionGUI/ImageDialog.h  VisionGUI/ImageDialog.cpp
	VisionGUI/ObjectToDraw.cpp VisionGUI/ObjectToDraw.h  VisionGUI/ClassifyImageDialog.cpp  VisionGUI/ClassifyImageDialog.h
	VisionGUI/ClassifyImageToolBox.cpp  VisionGUI/ClassifyImageToolBox.h

	main.cpp  mainwindow.cpp  mainwindow.h  MenuBar.h  MenuBar.cpp
	MotionWidget.h  MotionWindow.cpp  MotionWindow.h  MotionWidget.cpp
	DockWidget.cpp  DockWidget.h
	SubWindow.h  SubWindow.cpp  ToolBar.h ToolBar.cpp
  )



############################ CHECK LIBRARY / EXECUTABLE OPTION

IF( ${PROJECT_IS_REMOTE} STREQUAL "OFF" )
    MESSAGE( STATUS "${CUR_PROJECT_NAME} is compiled as dynamic object" )
    ADD_LIBRARY( ${CUR_PROJECT_NAME} SHARED ${PROJECT_SRCS} )
ELSE( ${PROJECT_IS_REMOTE} STREQUAL "OFF" )
    MESSAGE( STATUS "${CUR_PROJECT_NAME}is compiled as remote object" )
    ADD_EXECUTABLE( ${CUR_PROJECT_NAME} ${PROJECT_SRCS} )
ENDIF( ${PROJECT_IS_REMOTE} STREQUAL "OFF" )




############################ SET LIBRARIES TO LINK WITH
TARGET_LINK_LIBRARIES( ${CUR_PROJECT_NAME} ${ALCOMMON_LIBRARIES} ${PTHREAD_LIBRARIES} ${NEWMAT_LIBRARIES}  ${TINYXML_LIBRARIES} ${BOOST_LIBRARIES} ${QT_LIBRARIES} )



############################ BINARY COPY
# Copy automatically binary to output folder
GET_TARGET_PROPERTY( PROJECT_LOCATION ${CUR_PROJECT_NAME} LOCATION)
IF ( WIN32 )
    STRING(REGEX REPLACE "${CUR_PROJECT_NAME}.dll" "" PROJECT_LOCATION_SRC ${PROJECT_LOCATION})
    #SET (OUTPUT_ROOT_DIR_LIB ${AL_DIR}/modules/lib/ )
    FILE(TO_NATIVE_PATH ${PROJECT_LOCATION_SRC} PROJECT_LOCATION_SRC)
    FILE(TO_NATIVE_PATH ${OUTPUT_ROOT_DIR_LIB} OUTPUT_ROOT_DIR_LIB)
ENDIF ( WIN32 )


IF( ${PROJECT_IS_REMOTE} STREQUAL "OFF" )

IF (WIN32)
	  ADD_CUSTOM_COMMAND(
	    TARGET ${CUR_PROJECT_NAME}
	    POST_BUILD
	    COMMAND echo ${PROJECT_LOCATION_SRC}*
    	COMMAND echo ${OUTPUT_ROOT_DIR_LIB}
	    COMMAND xcopy  ${PROJECT_LOCATION_SRC}*.dll ${OUTPUT_ROOT_DIR_LIB} /Y /C
	  )
ELSE (WIN32)

  ADD_CUSTOM_COMMAND(
    TARGET ${CUR_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy
    ${PROJECT_LOCATION}
    ${OUTPUT_ROOT_DIR_LIB}
  )
 ENDIF (WIN32)
 ELSE( ${PROJECT_IS_REMOTE} STREQUAL "OFF" )


 ADD_CUSTOM_COMMAND(
    TARGET ${CUR_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_ROOT_DIR_EXE}/${CMAKE_CFG_INTDIR}
    COMMAND ${CMAKE_COMMAND}  -E copy ${PROJECT_LOCATION} ${OUTPUT_ROOT_DIR_EXE}/${CMAKE_CFG_INTDIR}
  )

ENDIF( ${PROJECT_IS_REMOTE} STREQUAL "OFF" )
