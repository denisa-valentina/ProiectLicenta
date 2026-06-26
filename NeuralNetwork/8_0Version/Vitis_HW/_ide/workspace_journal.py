# 2026-06-22T16:19:24.690528
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis_HW")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="hardware")
comp.build()

status = platform.build()

comp = client.get_component(name="software")
comp.build()

status = platform.build()

comp.build()

vitis.dispose()

