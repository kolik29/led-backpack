const EFFECTS = [
  {
    'label': 'Color shift',
    'effect': 'colorshift',
    'fields': [
      { 'type': 'select', 'name': 'style', 'label': 'Style', 'data': ['global', 'perpixel'], 'defaultValue': 'perpixel' },
      { 'type': 'number', 'name': 'changeEveryMs', 'label': 'Change every ms', 'defaultValue': 900 },
      { 'type': 'number', 'name': 'blendPerSecond', 'label': 'Blend per second', 'defaultValue': 120 },
    ],
  },
  {
    'label': 'Fire',
    'effect': 'fire',
    'fields': [
      { 'type': 'number', 'name': 'cooling', 'label': 'Cooling', 'defaultValue': 50 },
      { 'type': 'number', 'name': 'sparking', 'label': 'Sparking', 'defaultValue': 120 },
      { 'type': 'number', 'name': 'stepMs', 'label': 'Step', 'defaultValue': 30 },
      { 'type': 'number', 'name': 'maxFlares', 'label': 'Max Flares', 'defaultValue': 20 },
    ],
  },
  {
    'label': 'Flashlight',
    'effect': 'flashlight',
    'fields': [
      { 'type': 'color', 'name': 'color', 'label': 'Color', 'defaultValue': '#ffffff' },
    ],
  },
  {
    'label': 'Sparkles',
    'effect': 'sparkles',
    'fields': [
      { 'type': 'number', 'name': 'spawnPerSecond', 'label': 'Spawn per second', 'defaultValue': 30 },
      { 'type': 'number', 'name': 'fadePerSecond', 'label': 'fadePerSecond', 'defaultValue': 220 },
      { 'type': 'checkbox', 'name': 'randomColors', 'label': 'Random colors', 'defaultValue': true },
      { 'type': 'color', 'name': 'color', 'label': 'Color', 'defaultValue': '#ffffff' },
    ],
  },
  {
    'label': 'Plasma',
    'effect': 'plasma',
    'fields': [
      { 'type': 'number', 'name': 'speed', 'label': 'Speed', 'defaultValue': 10 },
      { 'type': 'number', 'name': 'scale', 'label': 'Scale', 'defaultValue': 28 },
      { 'type': 'number', 'name': 'contrast', 'label': 'Contrast', 'defaultValue': 1 },
    ],
  },
  {
    'label': 'Noise',
    'effect': 'noise',
    'fields': [
      { 'type': 'number', 'name': 'speed', 'label': 'Speed', 'defaultValue': 10 },
      { 'type': 'number', 'name': 'scale', 'label': 'Scale', 'defaultValue': 28 },
      { 'type': 'number', 'name': 'contrast', 'label': 'Contrast', 'defaultValue': 1 },
    ],
  },
  {
    'label': 'Matrix rain',
    'effect': 'matrixrain',
    'fields': [
      { 'type': 'color', 'name': 'color', 'label': 'Color', 'defaultValue': '#00ff00' },
    ],
  },
  {
    'label': 'Snake',
    'effect': 'snake',
    'fields': [
      { 'type': 'number', 'name': 'stepMs', 'label': 'stepMs', 'defaultValue': 140 },
      { 'type': 'number', 'name': 'len', 'label': 'length', 'defaultValue': 4 },
      { 'type': 'color', 'name': 'snakeColor', 'label': 'Color', 'defaultValue': '#00ff00' },
      { 'type': 'color', 'name': 'foodColor', 'label': 'Color', 'defaultValue': '#ff0000' }
    ],
  }
]

export default EFFECTS;