file(GLOB CORE
    "${SNTL_INCLUDE}/core/*.h"
    "${SNTL_SRC}/core/*.cpp"
)

file(GLOB ECS 
    "${SNTL_INCLUDE}/ecs/*.h"
    "${SNTL_SRC}/ecs/*.cpp"
)

file(GLOB SYS
    "${SNTL_INCLUDE}/sys/*.h"
    "${SNTL_SRC}/sys/*.cpp"
)

file(GLOB SYS_THREAD
    "${SNTL_INCLUDE}/sys/thread/*.h"
    "${SNTL_SRC}/sys/thread/*.cpp"
)

file(GLOB EVENTS
    "${SNTL_INCLUDE}/events/*.h"
    "${SNTL_SRC}/events/*.cpp"
)

file(GLOB WINDOW
    "${SNTL_INCLUDE}/window/*.h"
    "${SNTL_SRC}/window/*.cpp"
)

file(GLOB WINDOW_GLFW
    "${SNTL_INCLUDE}/window/glfw/*.h"
    "${SNTL_SRC}/window/glfw/*.cpp"
)

file(GLOB RENDERER
    "${SNTL_INCLUDE}/renderer/*.h"
    "${SNTL_SRC}/renderer/*.cpp"
)

source_group("core" FILES ${CORE})
source_group("ecs" FILES ${ECS})
source_group("sys" FILES ${SYS})
source_group("sys\\thread" FILES ${SYS_THREAD})
source_group("events" FILES ${EVENTS})
source_group("window" FILES ${WINDOW})
source_group("window\\glfw" FILES ${WINDOW_GLFW})
source_group("renderer" FILES ${RENDERER})