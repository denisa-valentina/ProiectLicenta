# 2026-06-12T19:48:25.817867
import vitis

client = vitis.create_client()
client.set_workspace(path="VITIS_HW")

platform = client.get_component(name="platform")
status = platform.build()

status = platform.build()

comp = client.get_component(name="app_component_sw")
comp.build()

status = platform.build()

comp.build()

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../../VIVADO_XSA/project_1/design_1_wrapper.xsa")

status = platform.build()

status = platform.build()

comp = client.get_component(name="app_component")
comp.build()

client.delete_component(name="platform")

platform = client.create_platform_component(name = "platform",hw_design = "$COMPONENT_LOCATION/../../VIVADO_XSA/project_1/design_1_wrapper.xsa",os = "standalone",cpu = "psu_cortexa53_0",domain_name = "standalone_psu_cortexa53_0")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

client.delete_component(name="platform")

client.delete_component(name="app_component_sw")

client.delete_component(name="app_component")

platform = client.create_platform_component(name = "platform",hw_design = "$COMPONENT_LOCATION/../../VIVADO_XSA/project_1/design_1_wrapper.xsa",os = "standalone",cpu = "psu_cortexa53_0",domain_name = "standalone_psu_cortexa53_0")

comp = client.create_app_component(name="hardware",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_psu_cortexa53_0")

comp = client.get_component(name="hardware")
status = comp.import_files(from_loc="", files=["/home/denisaa/Desktop/Licenta_Denisa_2026/ProiectLicenta/SQLInjection/FirstModel50/Train_Python/result_data.h", "/home/denisaa/Desktop/Licenta_Denisa_2026/ProiectLicenta/SQLInjection/FirstModel50/Train_Python/test_data.h"])

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../Train_Python", files=["result_data.h", "test_data.h"], dest_dir_in_cmp = "src")

comp = client.create_app_component(name="software",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_psu_cortexa53_0")

comp = client.get_component(name="software")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../Train_Python", files=["output.h", "result_data.h", "test_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp = client.get_component(name="hardware")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="software")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="hardware")
comp.build()

status = platform.build()

comp = client.get_component(name="software")
comp.build()

status = platform.build()

comp = client.get_component(name="hardware")
comp.build()

status = platform.build()

comp = client.get_component(name="software")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="hardware")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="software")
comp.build()

status = platform.build()

comp.build()

vitis.dispose()

