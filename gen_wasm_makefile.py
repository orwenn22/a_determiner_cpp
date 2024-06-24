import os


def add_source_files_to_array(directory: str, array: list):
    directory_list = os.listdir(directory)

    for element in directory_list:
        element_path = directory + "/" + element
        if os.path.isdir(element_path):
            add_source_files_to_array(element_path, array)
        elif os.path.isfile(element_path):
            if element_path.endswith(".c") or element_path.endswith(".cpp"):
                array.append(element_path)


# Non-recursive, add all files in the directory in the array
def add_files_to_array(directory: str, array: list):
    directory_list = os.listdir(directory)
    for element in directory_list:
        element_path = directory + "/" + element
        if os.path.isfile(element_path):
            array.append(element_path)


def mangle_to_object_file_name(source_file_path: str) -> str:
    return build_dir + "/" + source_file_path.replace("/", "_").replace(".", "_") + ".o"


#################################
# CONFIGURATION

source_dir = "./src"
preload_dirs = ["./res", "./maps"]
target_name = "adeterminer.html"
build_dir = "build_wasm"
makefile_name = "wasm_auto.Makefile"
CXX_command = "emcc -c [src] -o [obj] -Isrc -Iraylib/src"
LINK_command = "emcc [obj_list] ./raylib/src/libraylib.a -o [target] -lidbfs.js -s USE_GLFW=3 --shell-file shell.html -sGL_ENABLE_GET_PROC_ADDRESS [preloaded_files]"


#################################
# GENERATING MAKEFILE

out_file = open(makefile_name, "w")
source_files: list[str] = []
object_files = []
preload_files = []

# Main target
out_file.write(f"main: {target_name}\n\techo done\n\n")

# Create build dir
if not os.path.exists(build_dir):
    os.mkdir(build_dir)

# List all source files
add_source_files_to_array(source_dir, source_files)

# Compile source files
for source_file in source_files:
    object_file = mangle_to_object_file_name(source_file)       # Mangle to object file name
    object_files.append(object_file)

    command = CXX_command.replace("[src]", source_file).replace("[obj]", object_file)
    out_file.write(f"{object_file}: {source_file}\n\t{command}\n\n")

# Get preloaded files
for directory in preload_dirs:
    add_files_to_array(directory, preload_files)

# Add correct argument before every preloaded files
for i in range(0, len(preload_files)):
    preload_files[i] = "--preload-file " + preload_files[i]
preloaded_files_in_command = " ".join(preload_files)

# Link everything
objects = " ".join(object_files)
command = (LINK_command
           .replace("[obj_list]", objects).replace("[target]", target_name)
           .replace("[preloaded_files]", preloaded_files_in_command))

out_file.write(f"{target_name}: {objects}\n\t{command}\n\n")

# We are done, close the file
out_file.close()
