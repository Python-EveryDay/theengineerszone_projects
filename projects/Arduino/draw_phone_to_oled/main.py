import serial
from fastapi.responses import JSONResponse
from fastapi import FastAPI, Request
from fastapi.templating import Jinja2Templates



import uvicorn

app = FastAPI()

templates = Jinja2Templates(directory="templates")

import os

print("Current Directory:", os.getcwd())

@app.on_event("startup")
async def startup():

    global ser

    ser = serial.Serial(
        port="COM7",
        baudrate=115200,
        timeout=1
    )

    print("Serial Connected")
@app.get("/")
async def home(request: Request):
    return templates.TemplateResponse(
        request=request,
        name="index.html"
    )


@app.post("/draw")
async def draw(data: dict):

    x1 = int(data["x1"] * 128 / data["width"])
    y1 = int(data["y1"] * 64 / data["height"])

    x2 = int(data["x2"] * 128 / data["width"])
    y2 = int(data["y2"] * 64 / data["height"])

    msg = f"L,{x1},{y1},{x2},{y2}\n"

    print(msg.strip())

    ser.write(msg.encode())

    return {"status": "ok"}


@app.post("/clear")
async def clear():
    ser.write(b"C\n")
    return {"status": "ok"}

if __name__ == "__main__":
    uvicorn.run("main:app", host="0.0.0.0", port=8000, reload=False)