
from tencentcloud.common import credential
from tencentcloud.common.profile.client_profile import ClientProfile
from tencentcloud.common.profile.http_profile import HttpProfile
from tencentcloud.common.exception.tencent_cloud_sdk_exception import TencentCloudSDKException
from tencentcloud.bda.v20200324 import bda_client, models
import json
test_str = ""
import os
print(os.getcwd())
Errno = {
    "ERR_KEY_ID": 1,
    "ERR_NETWORK": 2,
    "ERR_IMAGE": 3,
    "ERR_UNKNOWN": 4,
    "ERR_NOBODY_IN_PHOTO": 5
}

KeyPointType = {
	"头部":0x0,
	"颈部":0x1,
	"右肩":0x2 | 0x10,
    "右肘":0x3 | 0x10,
    "右腕":0x4 | 0x10,
	"左肩":0x2 | 0x20,
    "左肘":0x3 | 0x20,
    "左腕":0x4 | 0x20,
    "右髋":0x5 | 0x10,
    "右膝":0x6 | 0x10,
    "右踝":0x7 | 0x10,
    "左髋":0x5 | 0x20,
    "左膝":0x6 | 0x20,
    "左踝":0x7 | 0x20,
};
print("Successfully imported this module.")


def fetch(imgb64, s_id, key):
    try:
        cred = credential.Credential(s_id, key)
        httpProfile = HttpProfile()
        httpProfile.endpoint = "bda.tencentcloudapi.com"

        clientProfile = ClientProfile()
        clientProfile.httpProfile = httpProfile
        client = bda_client.BdaClient(cred, "ap-beijing", clientProfile)

        req = models.DetectBodyJointsRequest()
        params = '{"Image":"' + imgb64 + '"}'
        req.from_json_string(params)

        resp = client.DetectBodyJoints(req)
        dict = json.loads(resp.to_json_string())
        for i in dict["BodyJointsResults"]:
            for j in i["BodyJoints"]:
                j["KeyPointType"] = KeyPointType[j["KeyPointType"]];
#       print(dict)
        return dict

       

    except TencentCloudSDKException as err:
        if err.code == "ClientNetworkError":
            return {"Error": Errno["NETWORK"]}
        elif err.code == "AuthFailure.SecretIdNotFound":
            return {"Error": Errno["ERR_KEY_ID"]}
        elif err.code == "FailedOperation.ImageDecodeFailed":
            return {"Error": Errno["ERR_IMAGE"]}
        elif err.code == "FailedOperation.NoBodyInPhoto":
            return {"Error": Errno["ERR_NOBODY_IN_PHOTO"]}
        else:
            print("Unknown error occurred: " + err.code)
            return {"Error": Errno["ERR_UNKNOWN"]}


################################
# print(fetch(test_str, "AKIDv4ze1BArRlFTUENMp43zjJhcHiso2Stq", "97ppY0MUlma8cuxpSGoGTN1Qa40hGagq"))
