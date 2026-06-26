# 2026-06-21T12:20:13.242227
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis_HW")

platform = client.get_component(name="platform")
status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../../Vivado_XSA/project_1/design_1_wrapper.xsa")

status = platform.build()

status = platform.build()

comp = client.get_component(name="hardware")
comp.build()

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../Python", files=["test_data.h", "result_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

client.delete_component(name="hardware")

comp = client.create_app_component(name="hardware",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_psu_cortexa53_0")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../Python", files=["test_data.h", "result_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

client.delete_component(name="hardware")

client.delete_component(name="platform")

platform = client.create_platform_component(name = "platform",hw_design = "$COMPONENT_LOCATION/../../Vivado_XSA/project_1/design_1_wrapper.xsa",os = "standalone",cpu = "psu_cortexa53_0",domain_name = "standalone_psu_cortexa53_0")

status = platform.build()

comp = client.create_app_component(name="hardware",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_psu_cortexa53_0")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../Python", files=["test_data.h", "result_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="software")
comp.build()

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../Python", files=["test_data.h", "result_data.h"], dest_dir_in_cmp = "src")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../Python", files=["mlp_c_test_suite.c", "mlp_c.h", "mlp_c.c"], dest_dir_in_cmp = "src")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../Python/include", files=["k2c_tensor_include.h", "k2c_include.h", "k2c_helper_functions.c", "k2c_core_layers.c", "k2c_activations.c"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../Python/include", files=["k2c_tensor_include.h", "k2c_include.h", "k2c_helper_functions.c", "k2c_core_layers.c", "k2c_activations.c"], dest_dir_in_cmp = "src")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../Python", files=["mlp_c.h", "mlp_c.c"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

vitis.dispose()

