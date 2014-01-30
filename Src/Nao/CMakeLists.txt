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
PROJECT( Nao )
IF(QT_USE STREQUAL "ON")
INCLUDE( ${QT_USE_FILE} )
ENDIF(QT_USE STREQUAL "ON")
SET(CMD_POSITION "C:\\WINDOWS\\system32\\cmd.exe"\\C)
############################ DEFINITION
# Add here definition needed.
ADD_DEFINITIONS( ${ALCOMMON_DEFINITIONS} ${PTHREAD_DEFINITIONS} ${TINYXML_DEFINITIONS} ${BOOST_DEFINITIONS})
IF(QT_USE STREQUAL "ON")
ADD_DEFINITIONS(${QT_DEFINITIONS})
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${QT_LIBRARIES})
ENDIF(QT_USE STREQUAL "ON")



############################ INCLUDE DIRECTORY
# Define include directories
INCLUDE_DIRECTORIES( ${PROXIES_INCLUDE_DIR} ${ALCOMMON_INCLUDE_DIR} ${PTHREAD_INCLUDE_DIR}  ${TINYXML_INCLUDE_DIR} ${BOOST_INCLUDE_DIR} ../Src/)





############################ PROJECT SOURCES FILES
# Add here source files needed to compile this project
SET(PROJECT_SRCS
	MainModule.cpp MainModule.h main.cpp main.h TJArkNao.h

	../Module/Common/CommandMsg.h ../Module/Common/CommandMsg.cpp 	../Module/Common/CommonData.h 	../Module/Common/FrameInfo.h 	
	../Module/Common/FrameInfo.cpp ../Module/Common/Geometry.h 		../Module/Common/Geometry.cpp 	../Module/Common/JointData.h 	
	../Module/Common/MotionCmd.h ../Module/Common/NaoConfig.h 		../Module/Common/NaoConfig.cpp ../Module/Common/NaoStructure.cpp 	
	../Module/Common/NaoStructure.h 	../Module/Common/RingBuffer.h 	../Module/Common/SmartPtr.h ../Module/Common/WalkCmd.h 		
	../Module/Common/WalkingEngineOutpu.h  ../Module/Common/VisionHeader.h 
	
	../Module/Core/MessageQueue.h  ../Module/Core/MessageQueue.cpp  ../Module/Core/Thread.h  ../Module/Core/Thread.cpp

	../Module/Location/Loc.h  ../Module/Location/Loc.cpp

	../Module/Motion/LIPMWalk.h  ../Module/Motion/LIPMWalk.cpp  ../Module/Motion/Motion.h  ../Module/Motion/Motion.cpp  	
	../Module/Motion/MotionEngine.h  ../Module/Motion/MotionEngine.cpp ../Module/Motion/MotionProvider.h  	
	../Module/Motion/MotionProvider.cpp  ../Module/Motion/WalkingEngine.h  ../Module/Motion/WalkingEngine.cpp  	
	../Module/Motion/WalkTypeBase.h ../Module/Motion/Tools/Kinematics.h ../Module/Motion/Tools/Kinematics.cpp

	../Module/Network/common.h  ../Module/Network/CommDef.h  ../Module/Network/Network.h  ../Module/Network/Network.cpp  
	../Module/Network/NetworkProvider.h  ../Module/Network/NetworkProvider.cpp  ../Module/Network/TcpConnect.h  
	../Module/Network/TcpConnect.cpp  ../Module/Network/TcpPoint.h  ../Module/Network/TcpPoint.cpp 
	../Module/Network/UdpConnect.h ../Module/Network/UdpConnect.cpp  ../Module/Network/UdpPoint.h  ../Module/Network/UdpPoint.cpp  
	../Module/Network/WirelessConfig.h  ../Module/Network/networkComm/networkInclude.h 
	../Module/Network/networkComm/NetworkMsg.cpp  ../Module/Network/networkComm/NetworkMsg.h  
	../Module/Network/networkComm/wdatainfo.cpp  ../Module/Network/networkComm/wdatainfo.h

	../Module/Representations/BaseProvider.h  ../Module/Representations/Blackboard.cpp  ../Module/Representations/Blackboard.h  
	../Module/Representations/KS.cpp  ../Module/Representations/KS.h  ../Module/Representations/KsComm.h 
	../Module/Representations/KSManager.cpp  ../Module/Representations/KSManager.h   
	../Module/Representations/Infrastructure/BehaviorData.h
	../Module/Representations/Infrastructure/Image.h  ../Module/Representations/Infrastructure/JointData.h
	../Module/Representations/Infrastructure/NetworkData.h  ../Module/Representations/Infrastructure/Percept.h
	../Module/Representations/Infrastructure/RobotPos.h  ../Module/Representations/Infrastructure/SensorData.h
	../Module/Representations/MotionControl/MotionData.h  
	../Module/Representations/Perception/VisionInterface.cpp  ../Module/Representations/Perception/VisionInterface.h

	../Module/Sensor/NaoProvider.cpp  ../Module/Sensor/NaoProvider.h  ../Module/Sensor/Sensor.cpp  	../Module/Sensor/Sensor.h  
	../Module/Sensor/SensorProvider.cpp  ../Module/Sensor/SensorProvider.h

	../Module/Vision/BallSpecialist.h  ../Module/Vision/BallSpecialist.cpp  ../Module/Vision/BresenhamLineScan.cpp  
	../Module/Vision/BresenhamLineScan.h  ../Module/Vision/DebugDrawings.h  ../Module/Vision/GoalSpecialist.cpp  
	../Module/Vision/GoalSpecialist.h  ../Module/Vision/LineFinder.cpp  ../Module/Vision/LineFinder.h ../Module/Vision/Run.h  
	../Module/Vision/ScanVision.cpp  ../Module/Vision/ScanVision.h  ../Module/Vision/SensorComm.cpp  ../Module/Vision/SensorComm.h 
	../Module/Vision/TJImage.cpp  ../Module/Vision/TJImage.h  ../Module/Vision/Vision.cpp  ../Module/Vision/Vision.h  
	../Module/Vision/VisionProvider.cpp  ../Module/Vision/VisionProvider.h

	../Module/Behavior/RobotProvider.cpp  ../Module/Behavior/RobotProvider.h  ../Module/Behavior/TJRobot.cpp  ../Module/Behavior/TJRobot.h
	#../Module/Behavior/RunBehav.cpp  ../Module/Behavior/RunBehav.h  

	../Tools/Math/Common.h  ../Tools/Math/Matrix.cpp  ../Tools/Math/Matrix.h  ../Tools/Math/Pose2D.h  ../Tools/Math/Pose3D.h  
	../Tools/Math/Range.h  ../Tools/Math/Vector2.h  ../Tools/Math/Vector3.h  ../Tools/Streams/InOut.cpp  ../Tools/Streams/InOut.h 
	../Tools/Streams/InStreams.cpp  ../Tools/Streams/InStreams.h  ../Tools/Streams/OutStreams.cpp  ../Tools/Streams/OutStreams.h  
	../Tools/Streams/Streamable.cpp  ../Tools/Streams/Streamable.h  ../Tools/Streams/StreamHandler.cpp  
	../Tools/Streams/StreamHandler.h
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
TARGET_LINK_LIBRARIES( ${CUR_PROJECT_NAME} ${ALCOMMON_LIBRARIES} ${PTHREAD_LIBRARIES} ${NEWMAT_LIBRARIES}  ${TINYXML_LIBRARIES} ${BOOST_LIBRARIES} )



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
