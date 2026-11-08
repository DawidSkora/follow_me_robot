import cv2, time, os
from enum import Enum
from ultralytics import YOLO
from collections import deque 

FRAME_SIZE = 320
DIRECTION_MARGIN = 50
MAX_FRAMES_WITHOUT_DETECTION = 10
MAX_BOX_TO_FRAME_RATIO = 0.5


offset_x_history = deque([0,0,0,0,0], maxlen=5)

class Direction(Enum):
    LEFT = "LEFT"
    RIGHT = "RIGHT"
    FORWARD = "FORWARD"
    STOP_NO_DETECTION = "STOP_NO_OBJ"
    STOP_MIN_DISTANCE = "STOP_MIN_DIST"


def prepare_model():
    #if not os.path.exists('yolov8n_ncnn_model'):
    #    model = YOLO('yolov8n.pt')
    #    model.export(format='ncnn')
    #
    #model = YOLO('yolov8n_ncnn_model', task='detect')    
    model = YOLO('yolov8n.pt')
    return model

def get_biggest_box_coord(boxes):
    max_box_field = 0
    biggest_box_id = 0
    for i, box in enumerate(boxes):
        x1, y1, x2, y2 = box.xyxy[0]
        field = (x2-x1)*(y2-y1)
        if field > max_box_field:
            max_box_field = field
            biggest_box_id = i
    x1, y1, x2, y2 = boxes[biggest_box_id].xyxy[0]
    cx = int((x1 + x2) / 2)
    cy = int((y1 + y2) / 2)
    return cx, cy, max_box_field

def recalculate_offset(cx, frame_center_x):
    offset_x_history.append(cx - frame_center_x)
    return sum(offset_x_history) / len(offset_x_history)

def main():
    model = prepare_model()

    cap = cv2.VideoCapture(0)
    prev_time = 0
    frames_without_detection = 0
    direction = Direction.STOP_NO_DETECTION
    while True:
        if frames_without_detection > MAX_FRAMES_WITHOUT_DETECTION:
            direction = Direction.STOP_NO_DETECTION
        current_time = time.time()

        ret, frame = cap.read()
        frame_height, frame_width = frame.shape[:2]
        frame_area = frame_height * frame_width
        frame_center_x = frame_width // 2
        
        results = model(frame, imgsz=FRAME_SIZE, classes=[0])
        annotated_frame = results[0].plot()
        
        boxes = results[0].boxes
        if boxes:
            frames_without_detection = 0
            cx, cy, box_area = get_biggest_box_coord(boxes)

            offset_avg = recalculate_offset(cx, frame_center_x)

            if box_area / frame_area < MAX_BOX_TO_FRAME_RATIO:
                if offset_avg < -DIRECTION_MARGIN:
                    direction = Direction.LEFT
                elif offset_avg > DIRECTION_MARGIN:
                    direction = Direction.RIGHT
                else:
                    direction = Direction.FORWARD
            else:
                direction = Direction.STOP_MIN_DISTANCE

            cv2.putText(annotated_frame, f"X ", (cx-5, cy+5),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
            cv2.putText(annotated_frame, f"Of avg:{offset_avg}, cx:{cx}", (10, 90),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
        else:
            frames_without_detection = frames_without_detection + 1
        
        cv2.putText(annotated_frame, f"{direction.value}", (10, 60),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

        fps = 1 / (current_time - prev_time)
        cv2.putText(annotated_frame, f"{fps:.1f}", (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        
        cv2.imshow('YOLO Detekcja', annotated_frame)
        
        prev_time = current_time
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()