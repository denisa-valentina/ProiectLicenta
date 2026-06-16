# 2026-06-12T21:32:12.275388
import vitis

client = vitis.create_client()
client.set_workspace(path="VITIS_HW")

platform = client.get_component(name="platform")
status = platform.build()

status = platform.build()

comp = client.get_component(name="software")
comp.build()

status = platform.build()

comp = client.get_component(name="hardware")
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

vitis.dispose()

