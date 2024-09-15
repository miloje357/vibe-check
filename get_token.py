import base64
import json
import os
import sys
import webbrowser

import requests


def save_tokens(token_filepath, access_token=None, refresh_token=None):
    token_json = {}
    token_filepath = os.path.expanduser(token_filepath)
    if os.path.exists(token_filepath):
        try:
            with open(token_filepath, "r") as token_file:
                token_json = json.load(token_file)
        except json.JSONDecodeError:
            pass

    if access_token:
        token_json["access_token"] = access_token
    if refresh_token:
        token_json["refresh_token"] = refresh_token

    with open(token_filepath, "w") as token_file:
        json.dump(token_json, token_file, indent=4)


def load_tokens(token_filepath):
    token_filepath = os.path.expanduser(token_filepath)
    if not os.path.exists(token_filepath):
        return None
    try:
        with open(token_filepath, "r") as token_file:
            token_json = json.load(token_file)
    except json.JSONDecodeError:
        return None
    if not ("access_token" in token_json and "refresh_token" in token_json):
        return None
    return token_json["access_token"], token_json["refresh_token"]


def is_valid_token(token):
    url = "https://api.spotify.com/v1/me/player/currently-playing?market=RS&additional_types=track"
    headers = {"Authorization": f"Bearer {token}"}
    res = requests.request("GET", url, headers=headers)
    return res.status_code == 200


def get_firsttime_token(client_id, client_secret):
    webbrowser.open(
        f"https://accounts.spotify.com/authorize?client_id={client_id}&response_type=code&redirect_uri=http%3A%2F%2Flocalhost%3A8888%2Fcallback&scope=user-read-currently-playing"
    )
    code_url = input("Copy-paste the url you were redirected to: ")

    if "?code=" not in code_url:
        print("Error ocurred: Couldn't find code")
        return
    code = code_url.split("?code=", maxsplit=1)[1]

    url = f"https://accounts.spotify.com/api/token?grant_type=authorization_code&code={code}&redirect_uri=http://localhost:8888/callback"
    auth = base64.b64encode(bytes(f"{client_id}:{client_secret}", "utf-8")).decode()
    headers = {
        "Content-Type": "application/x-www-form-urlencoded",
        "Authorization": f"Basic {auth}",
    }
    res = requests.request("POST", url, headers=headers)
    if res.status_code != 200:
        print(f"Error occured: {res.json()['error_description']}")
        return
    return res.json()["access_token"], res.json()["refresh_token"]


def refresh_token(rtoken, client_id, client_secret):
    url = f"https://accounts.spotify.com/api/token?grant_type=refresh_token&refresh_token={rtoken}"
    auth = base64.b64encode(bytes(f"{client_id}:{client_secret}", "utf-8")).decode()
    headers = {
        "Content-Type": "application/x-www-form-urlencoded",
        "Authorization": f"Basic {auth}",
    }
    res = requests.request("POST", url, headers=headers)

    if res.status_code != 200:
        print(f"Error occured: {res.json()['error_description']}")
        return

    return res.json()["access_token"]


def firsttime(token_filepath, client_id, client_secret):
    tokens = get_firsttime_token(client_id, client_secret)
    if not tokens:
        exit(1)
    access_token, refresh_token = tokens
    save_tokens(token_filepath, access_token=access_token, refresh_token=refresh_token)
    return access_token


def check_tokens(token_filepath, client_id, client_secret):
    tokens = load_tokens(token_filepath)
    if not tokens:
        return
    access_token, rtoken = tokens
    if is_valid_token(access_token):
        return access_token
    access_token = refresh_token(rtoken, client_id, client_secret)
    if not access_token:
        return
    save_tokens(token_filepath, access_token=access_token)
    return access_token


def create_credentials(filepath):
    filepath = os.path.expanduser(filepath)
    creds = {}
    creds["client_id"] = input("Client ID: ")
    creds["client_secret"] = input("Client Secret: ")
    with open(filepath, "w") as cred_file:
        json.dump(creds, cred_file)


def get_credentials(filepath):
    filepath = os.path.expanduser(filepath)
    if not os.path.exists(filepath):
        create_credentials(filepath)
    try:
        with open(filepath, "r") as creds_file:
            creds = json.load(creds_file)
    except json.JSONDecodeError:
        create_credentials(filepath)
        return get_credentials(filepath)
    if "client_id" not in creds or "client_secret" not in creds:
        create_credentials(filepath)
        return get_credentials(filepath)
    return creds


if __name__ == "__main__":
    if len(sys.argv) == 1 or len(sys.argv) > 3:
        print(
            "Please provide the root directory:\n\n\tpython get_token.py ~/.cache/vibe_check\n"
        )
        exit(1)
    token_filepath = sys.argv[-1] + "tokens.json"
    credentials_filepath = sys.argv[-1] + "credentials.json"
    creds = get_credentials(credentials_filepath)
    if len(sys.argv) == 3 and sys.argv[1] == "firsttime":
        firsttime(token_filepath, creds["client_id"], creds["client_secret"])
        exit(0)
    if len(sys.argv) == 2:
        token = check_tokens(token_filepath, creds["client_id"], creds["client_secret"])
        exit(token is not None)
