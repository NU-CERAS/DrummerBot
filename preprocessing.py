import mido
from mido import Message, MidiFile, MidiTrack
from collections import defaultdict

def preprocess_midi(midi_file, drum_delays):
    mid = mido.MidiFile(midi_file)
    
    adjusted_events = defaultdict(list)
    
    current_time = 0
    for msg in mid:
        current_time += msg.time
        
        if msg.type == 'note_on':  # 9 is drum notes
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

def printMotorControls(commands):
    for time, note, velocity in commands:
        print("Time: " + str(time) + "\t" + "Note: " + str(note) + "\t" + "Velocity: " + str(velocity))

def getBPM(midi_file):
    default_bpm = 120
    for track in midi_file.tracks:
        for msg in track:
            if msg.type == 'set_tempo':
                return mido.tempo2bpm(msg.tempo)
    return default_bpm  # Return default BPM if no tempo event is found

def createProcessedMidi(commands, bpm):
    midi = MidiFile()
    track = MidiTrack()
    midi.tracks.append(track)

    for time, note, velocity in commands:
        ticks = int(time * midi.ticks_per_beat * bpm / 60) 

        if velocity > 0:  # Note on
            track.append(Message('note_on', note=note, velocity=velocity, time=ticks))
        else:  # Note off
            track.append(Message('note_off', note=note, velocity=0, time=ticks))

    # Save the MIDI file
    midi.save('output.mid')


def main():
    # Example usage
    drum_delays = {
        36: .1,
        37: .2,
        38: .1,
        39: .1
        # 36: 0.05,  # Bass drum
        # 38: 0.03,  # Snare drum
        # 42: 0.02,  # Closed hi-hat
        # 46: 0.04,  # Open hi-hat
        # 49: 0.06,  # Crash cymbal
    }
    max_delta = max(drum_delays.values())


    midiname = r'DrummerBot\midifile.mid'
    midi_file = mido.MidiFile(r'DrummerBot\midifile.mid')  # Replace with your actual MIDI file path


    # preprocess adjusted times of hits
    bpm = getBPM(midi_file)
    adjusted_events = preprocess_midi(midiname, drum_delays)
    motor_commands = generate_motor_commands(adjusted_events, max_delta)
    printMotorControls(motor_commands)
    createProcessedMidi(motor_commands, bpm)


if __name__ == "__main__":
    main()