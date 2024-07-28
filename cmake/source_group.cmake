file(GLOB CORE
    "${SNTL_INCLUDE}/core/*.h"
    "${SNTL_SRC}/core/*.cpp"
)

file(GLOB CORE_THREAD
    "${SNTL_INCLUDE}/core/thread/*.h"
    "${SNTL_SRC}/core/thread/*.cpp"
)

file(GLOB CORE_CODES
    "${SNTL_INCLUDE}/core/codes/*.h"
    "${SNTL_SRC}/core/codes/*.cpp"
)

file(GLOB ECS 
    "${SNTL_INCLUDE}/ecs/*.h"
    "${SNTL_SRC}/ecs/*.cpp"
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

file(GLOB RENDERER_GL
    "${SNTL_INCLUDE}/renderer/opengl/*.h"
    "${SNTL_SRC}/renderer/opengl/*.cpp"
)

source_group("core" FILES ${CORE})
source_group("core\\thread" FILES ${CORE_THREAD})
source_group("core\\codes" FILES ${CORE_CODES})
source_group("ecs" FILES ${ECS})
source_group("events" FILES ${EVENTS})
source_group("window" FILES ${WINDOW})
source_group("window\\glfw" FILES ${WINDOW_GLFW})
source_group("renderer" FILES ${RENDERER})
source_group("renderer\\opengl" FILES ${RENDERER_GL})