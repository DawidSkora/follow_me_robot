import cv2
import glob

devices = glob.glob('/dev/video*')

for dev in sorted(devices):
    index = int(dev.replace('/dev/video', ''))
    cap = cv2.VideoCapture(index)
    
    if cap.isOpened():
        ret, frame = cap.read()
        if ret and frame is not None:
            print(f"{dev} -> DZIAŁA, rozmiar klatki: {frame.shape}")
        else:
            print(f"{dev} -> otwiera się, ale nie zwraca klatki")
        cap.release()
    else:
        print(f"{dev} -> nie da się otworzyć")