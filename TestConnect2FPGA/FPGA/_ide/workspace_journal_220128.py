# 2026-05-21T19:02:04.621945
import vitis

client = vitis.create_client()
client.set_workspace(path="FPGA")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="app_component")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="xuartlite_low_level_example")
comp.build()

status = platform.build()

comp.build()

vitis.dispose()

