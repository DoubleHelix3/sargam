import os 
import random

def play(notes):
  dictionary = {'S':'C','r':'Db','R':'D','g':'Eb','G':'E',
  'M':'F','m':'F#','P':'G','d':'Ab','D':'A','n':'Bb','N':'B'}

  for note in notes:
    os.system('echo "'+dictionary[note]+'":0.2 | musicalbeeps > trash.txt')


def random_note():
  notes = list("SRGPD")

  note = random.choice(notes)

  while note.lower() == random_note.previous.lower() and note != previous:
    note = random.choice(notes)

  random_note.previous = note

  return note

random_note.previous = ''


previous = ''

play("S")

while True:
  notes = random_note() + random_note()
  while notes == previous:
    notes = random_note() + random_note()
  
  previous = notes

  play(notes)
  
  while input().lower() != notes.lower():
    print("Wrong")
    play(notes)

  print("Correct")

