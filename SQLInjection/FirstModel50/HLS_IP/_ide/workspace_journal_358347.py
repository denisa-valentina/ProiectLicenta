# 2026-06-12T15:51:31.160613
import vitis

client = vitis.create_client()
client.set_workspace(path="BrokenModel")

comp = client.get_component(name="hls_component")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="PACKAGE")

comp.run(operation="CO_SIMULATION")

vitis.dispose()

