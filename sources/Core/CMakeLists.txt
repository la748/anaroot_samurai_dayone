project(anacore)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS
src/TArtCore.cc
src/TArtDataSource.cc
src/TArtDecoderA3100.cc
src/TArtDecoderAD413A.cc
src/TArtDecoderAMTTDC.cc
src/TArtDecoderBigEndianV7XX.cc
src/TArtDecoderCTM405.cc
src/TArtDecoderDSP.cc
src/TArtDecoderFactory.cc
src/TArtDecoderFixed16.cc
src/TArtDecoderFixed24.cc
src/TArtDecoderFixed32.cc
src/TArtDecoderHINP.cc
src/TArtDecoderL3377d.cc
src/TArtDecoderLUPO.cc
src/TArtDecoderMADC32.cc
src/TArtDecoderMQDC32.cc
src/TArtDecoderMTDC32.cc
src/TArtDecoderNeulandTS.cc
src/TArtDecoderP716X.cc
src/TArtDecoderP716XZ.cc
src/TArtDecoderSIS3301.cc
src/TArtDecoderTacquila.cc
src/TArtDecoderV1190.cc
src/TArtDecoderV1190C.cc
src/TArtDecoderV1290.cc
src/TArtDecoderV1740.cc
src/TArtDecoderV550.cc
src/TArtDecoderV767.cc
src/TArtDecoderV7XX.cc
src/TArtDecoderV830.cc
src/TArtDecoderVmeEasyroc.cc
src/TArtEventStore.cc
src/TArtFileDataSource.cc
src/TArtMath.cc
src/TArtParser.cc
src/TArtParserFactory.cc
src/TArtParserRDF.cc
src/TArtParserRIDF.cc
src/TArtRawDataObject.cc
src/TArtRawEventObject.cc
src/TArtRawScalerObject.cc
src/TArtRawSegmentObject.cc
src/TArtSemaphore.cc
src/TArtSharedMemory.cc
src/TArtSharedMemoryDataSource.cc
src/TArtStoreManager.cc
src/TArtStreamingDataSource.cc
src/TArtUtil.cc
)

set(HEADERS
include/TArtCore.hh
include/TArtDataSource.hh
include/TArtDecoderA3100.hh
include/TArtDecoderAD413A.hh
include/TArtDecoderAMTTDC.hh
include/TArtDecoderBigEndianV7XX.hh
include/TArtDecoderCTM405.hh
include/TArtDecoderDSP.hh
include/TArtDecoderFactory.hh
include/TArtDecoderFixed16.hh
include/TArtDecoderFixed24.hh
include/TArtDecoderFixed32.hh
include/TArtDecoderHINP.hh
include/TArtDecoderL3377d.hh
include/TArtDecoderLUPO.hh
include/TArtDecoderMADC32.hh
include/TArtDecoderMQDC32.hh
include/TArtDecoderMTDC32.hh
include/TArtDecoderNeulandTS.hh
include/TArtDecoderP716X.hh
include/TArtDecoderP716XZ.hh
include/TArtDecoderSIS3301.hh
include/TArtDecoderTacquila.hh
include/TArtDecoderV1190.hh
include/TArtDecoderV1190C.hh
include/TArtDecoderV1290.hh
include/TArtDecoderV1740.hh
include/TArtDecoderV550.hh
include/TArtDecoderV767.hh
include/TArtDecoderV7XX.hh
include/TArtDecoderV830.hh
include/TArtDecoderVmeEasyroc.hh
include/TArtEventInfo.hh
include/TArtEventStore.hh
include/TArtFileDataSource.hh
include/TArtMath.hh
include/TArtParser.hh
include/TArtParserFactory.hh
include/TArtParserRDF.hh
include/TArtParserRIDF.hh
include/TArtRawDataObject.hh
include/TArtRawEventObject.hh
include/TArtRawScalerObject.hh
include/TArtRawSegmentObject.hh
include/TArtSemaphore.hh
include/TArtSharedMemory.hh
include/TArtSharedMemoryDataSource.hh
include/TArtStoreManager.hh
include/TArtStreamingDataSource.hh
include/TArtUtil.hh 
)

# for MINOS
option(enable_minos "enable minos library" off)
if(enable_minos)
 message("MINOS is enabled")
 find_package(PkgConfig)
 pkg_check_modules(MINOS_FEM REQUIRED minos-fem)

 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_MINOS")
 set(HEADERS ${HEADERS} 
 	    include/TArtDecoderFeminos.hh
	    include/TArtRawFeminosDataObject.hh
	    )
 set(SRCS ${SRCS}
	 src/TArtDecoderFeminos.cc	
	 )
 include_directories(${MINOS_FEM_INCLUDE_DIRS})
 ROOT_GENERATE_DICTIONARY(G__anacore
	${HEADERS}	
	LINKDEF anacore_linkdef.hh
	OPTIONS	"-DUSE_MINOS"
	)
else()
 message("MINOS is disabled")
 ROOT_GENERATE_DICTIONARY(G__anacore
	${HEADERS}	
	LINKDEF anacore_linkdef.hh
	)
endif()


add_library(anacore SHARED
		    ${SRCS}		    		    
	G__anacore.cxx
	)

target_link_libraries(anacore ${ROOT_LIBRARIES})
if(enable_minos)
 target_link_libraries(anacore "-L${MINOS_FEM_LIBRARY_DIRS}" ${MINOS_FEM_LIBRARIES})
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
		  DESTINATION include)

install(TARGETS anacore
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if (${ROOT_VERSION} VERSION_GREATER "6.0")
   install(
      FILES
      ${CMAKE_CURRENT_BINARY_DIR}/libanacore_rdict.pcm
      ${CMAKE_CURRENT_BINARY_DIR}/libanacore.rootmap
      DESTINATION lib/)
endif (${ROOT_VERSION} VERSION_GREATER "6.0")
