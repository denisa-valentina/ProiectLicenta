# 2026-06-18T18:03:29.272745
import vitis

client = vitis.create_client()
client.set_workspace(path="my-hls-test")

vitis.dispose()

