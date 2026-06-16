# 2026-05-30T12:38:07.590784
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis_After")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="software")
comp.build()

status = platform.build()

comp.build()

vitis.dispose()

