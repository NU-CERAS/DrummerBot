import mido
from collections import defaultdict

def preprocess_midi(midi_file, drum_delays):
    mid = mido.MidiFile(midi_file)
    
    adjusted_events = defaultdict(list)
    
    current_time = 0
    for msg in mid:
        current_time += msg.time
        
        if msg.type == 'note_on' and msg.channel == 9:  # 9 is drum notes
            note = msg.note
            if note in drum_delays:
                adjusted_time = current_time - drum_delays[note]
                adjusted_events[note].append((adjusted_time, msg.velocity))
    return adjusted_events

def generate_motor_commands(adjusted_events, max_delta):
    all_events = []
    for note, events in adjusted_events.items():
        for time, velocity in events: # events is a list of the adjusted time and velocity 
            all_events.append((time, note, velocity))
    
    all_events.sort()
    
    commands = []
    for time, note, velocity in all_events:
        commands.append([round(time + max_delta, 3), note, velocity])
        
    return commands

# def send_midi_data(motor_commands, port='/dev/ttyACM0', baudrate=115200):
#     try:
#         ser = serial.Serial(port, baudrate)
#         time.sleep(2)  # Allow time for Arduino reset

#         # Send the total number of commands first
#         num_commands = len(motor_commands)
#         ser.write(struct.pack('<I', num_commands))

#         # Send all commands at once
#         for hit in motor_commands:
#             data = struct.pack('<Ibb', int(hit[0]), hit[1], hit[2])
#             ser.write(data)

#         print(f"Sent {num_commands} commands to Arduino")

#     except serial.SerialException as e:
#         print(f"Serial error: {e}")
#     finally:
#         if ser.is_open:
#             ser.close()

def printMotorControls(commands):
    for time, note, velocity in commands:
        print("Time: " + str(time) + "\t" + "Note: " + str(note) + "\t" + "Velocity: " + str(velocity))


def main():
    # Example usage
    drum_delays = {
        36: 0.05,  # Bass drum
        38: 0.03,  # Snare drum
        42: 0.02,  # Closed hi-hat
        46: 0.04,  # Open hi-hat
        49: 0.06,  # Crash cymbal
    }
    max_delta = max(drum_delays.values())


    midi_file = r'DrummerBot\Rock33.mid'

    # preprocess adjusted times of hits
    adjusted_events = preprocess_midi(midi_file, drum_delays)
    motor_commands = generate_motor_commands(adjusted_events, max_delta)
    print(len(adjusted_events))
    printMotorControls(motor_commands)


if __name__ == "__main__":
    main()