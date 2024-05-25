# Кодировка Unicode
add_compile_definitions(_UNICODE UNICODE)

# Минимальная версия CMake
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
		set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Имя проекта
project ("Server")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Server "Server.cpp" "Server.h" "ServerNetwork.cpp" "ServerNetwork.hpp" "Interface.cpp" "Interface.hpp")

# Если CMake версии 3.12 и новее то используем стандарт C++17
if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET Server PROPERTY CXX_STANDARD 17)
endif()

# Добавляем шрифт
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/fonts/arial.ttf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Используем PThread (Потоки POSIX)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# Подключаем зависимости
add_subdirectory(dependencies)

# Подключаем необходимые библиотеки
target_link_libraries(Server PUBLIC ImGui-SFML::ImGui-SFML)
target_link_libraries(Server PUBLIC sfml-network)
target_link_libraries(Server PUBLIC Threads::Threads)
target_link_libraries(Server PUBLIC OpenSSL::SSL)
target_link_libraries(Server PUBLIC OpenSSL::Crypto)