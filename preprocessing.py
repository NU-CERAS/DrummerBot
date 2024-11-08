import mido
from mido import Message, MidiFile, MidiTrack
from collections import defaultdict

# Finds all Note numbers used in midi track
def getNotes(mid):
    notes = set() 
    for msg in mid:        
        if msg.type == 'note_on':
            note = msg.note
            notes.add(note)
    return notes

# Prints out motor commands in Time, Note, Velocity format
def printCommands(commands):
    for time, note, velocity in commands:
        print("Time: " + str(time) + "\t" + "Note: " + str(note) + "\t" + "Velocity: " + str(velocity))

# Returns bpm for midi track. If none is found, returns a default bpm
def getBPM(midi_file):
    for track in midi_file.tracks:
        for msg in track:
            if msg.type == 'set_tempo':
                return mido.tempo2bpm(msg.tempo)
    return 120 # if nothing found, return 120bpm 

# Processes midi file and pushes back drum hits by given amount so they create sound at the right time
def preprocess_midi(mid):        
    adjusted_events = defaultdict(list)
    current_time = 0
    for msg in mid:
        current_time += msg.time
        if msg.type == 'note_on':  
            note = msg.note
            vel = msg.velocity
            adjusted_time = current_time - (vel / 1000)
            adjusted_events[note].append((adjusted_time, msg.velocity))
        elif msg.type =='note_off':
            note = msg.note
            vel = msg.velocity
            adjusted_time = current_time - (vel / 1000)
            adjusted_events[note].append((adjusted_time, msg.velocity))
        
    all_events = []
    for note, events in adjusted_events.items():
        for time, velocity in events: # events is a list of the adjusted time and velocity 
            all_events.append((time, note, velocity))

    all_events.sort()
    max_delta = 0
    for i in all_events:
        max_delta = min(max_delta, i[0])
    commands = []
    for time, note, velocity in all_events:
        commands.append([round(time + max_delta, 5), note, velocity])
    return commands

# Creates midi file based on commands
def createProcessedMidi(commands, bpm):
    midi = MidiFile()
    track = MidiTrack()
    midi.tracks.append(track)
    prev_time = commands[0][0]

    for time, note, velocity in commands:
        ticks = int((time - prev_time) * midi.ticks_per_beat * bpm / 60)
        # print("Ticks: " + str(ticks) + "\t" + "Note: " + str(note) + "\t" + "Velocity: " + str(velocity))

        if velocity > 0:  # Note on
            track.append(Message('note_on', channel=9, note=note, velocity=velocity, time=ticks))
        else:  # Note off
            track.append(Message('note_off', channel=9, note=note, velocity=0, time=ticks))
        prev_time = time
    midi.save('OUTPUT.mid')

def main():
    # MIDI FILE PATH
    midi_file = mido.MidiFile(r'test Edit 1 Export 1.mid')  
    commands = preprocess_midi(midi_file)
    createProcessedMidi(commands, getBPM(midi_file))      

if __name__ == "__main__":
    main()