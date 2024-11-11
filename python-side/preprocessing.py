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
    adjusted_events = []
    current_time = 0
    note_end_shift_time = {}

    for msg in mid:
        current_time += msg.time
        adjusted_time = current_time - (msg.velocity / 1000) if (msg.type == 'note_on' or msg.type == 'note_off') else 0

        if msg.type == 'note_on':  
            note_end_shift_time[(msg.note, msg.channel)] = current_time - adjusted_time
            adjusted_events.append((adjusted_time, msg.note, msg.velocity))
        elif msg.type =='note_off':
            adjusted_time = current_time - note_end_shift_time.pop((msg.note, msg.channel), 0)
            adjusted_events.append((adjusted_time, msg.note, msg.velocity))

    adjusted_events.sort()
    max_negative = min(0, adjusted_events[0][0])
    adjusted_events = [(round(time + (max_negative * -1), 5), note, velocity) for time, note, velocity in adjusted_events]
    return adjusted_events

# Creates midi file based on commands
def createProcessedMidi(adjusted_events, bpm):
    midi = MidiFile()
    track = MidiTrack()
    midi.tracks.append(track)
    prev_time = adjusted_events[0][0]

    for time, note, velocity in adjusted_events:
        ticks = int((time - prev_time) * midi.ticks_per_beat * bpm / 60)
        if velocity > 0:  # Note on
            track.append(Message('note_on', channel=9, note=note, velocity=velocity, time=ticks))
        else:  # Note off
            track.append(Message('note_off', channel=9, note=note, velocity=0, time=ticks))
        prev_time = time
    midi.save('OUTPUT.mid')

def main():
    # MIDI FILE PATH
    midi_file = mido.MidiFile(r'waveformpyramid.mid')  
    adjusted_events = preprocess_midi(midi_file)
    createProcessedMidi(adjusted_events, getBPM(midi_file))      

if __name__ == "__main__":
    main()