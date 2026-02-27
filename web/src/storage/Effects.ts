const EFFECTS = [
  {
    'label': 'Color shift',
    'effect': 'colorshift',
    'fields': [
      { 'name': 'style', 'type': 'select', 'label': 'Style', 'data': ['global', 'perpixel'], 'defaultValue': 'perpixel' },
      { 'name': 'changeEveryMs', 'type': 'number', 'label': 'Change every ms', 'defaultValue': 900 },
      { 'name': 'blendPerSecond', 'type': 'number', 'label': 'Blend per second', 'defaultValue': 120 },
    ],
  },
  {
    'label': 'Fire',
    'effect': 'fire',
    'fields': [
      { 'name': 'cooling', 'type': 'number', 'label': 'Cooling', 'defaultValue': 50 },
      { 'name': 'sparking', 'type': 'number', 'label': 'Sparking', 'defaultValue': 120 },
      { 'name': 'stepMs', 'type': 'number', 'label': 'Step', 'defaultValue': 30 },
      { 'name': 'maxFlares', 'type': 'number', 'label': 'Max Flares', 'defaultValue': 20 },
    ],
  },
  {
    'label': 'Flashlight',
    'effect': 'flashlight',
    'fields': [
      { 'name': 'color', 'type': 'color', 'label': 'Color', 'defaultValue': '#ffffff' },
    ],
  },
  {
    'label': 'Sparkles',
    'effect': 'sparkles',
    'fields': [
      { 'name': 'spawnPerSecond', 'type': 'number', 'label': 'Spawn per second', 'defaultValue': 30 },
      { 'name': 'fadePerSecond', 'type': 'number', 'label': 'Fade per second', 'defaultValue': 220 },
      { 'name': 'randomColors', 'type': 'checkbox', 'label': 'Random colors', 'defaultValue': true },
      { 'name': 'color', 'type': 'color', 'label': 'Color', 'defaultValue': '#ffffff' },
    ],
  },
  {
    'label': 'Plasma',
    'effect': 'plasma',
    'fields': [
      { 'name': 'speed', 'type': 'number', 'label': 'Speed', 'defaultValue': 10 },
      { 'name': 'scale', 'type': 'number', 'label': 'Scale', 'defaultValue': 28 },
      { 'name': 'contrast', 'type': 'number', 'label': 'Contrast', 'defaultValue': 1 },
    ],
  },
  {
    'label': 'Noise',
    'effect': 'noise',
    'fields': [
      { 'name': 'speed', 'type': 'number', 'label': 'Speed', 'defaultValue': 10 },
      { 'name': 'scale', 'type': 'number', 'label': 'Scale', 'defaultValue': 28 },
      { 'name': 'contrast', 'type': 'number', 'label': 'Contrast', 'defaultValue': 1 },
    ],
  },
  {
    'label': 'Matrix rain',
    'effect': 'matrixrain',
    'fields': [
      { 'name': 'color', 'type': 'color', 'label': 'Color', 'defaultValue': '#00ff00' },
    ],
  },
  {
    'label': 'Snake',
    'effect': 'snake',
    'fields': [
      { 'name': 'stepMs', 'type': 'number', 'label': 'Step (ms)', 'defaultValue': 140 },
      { 'name': 'len', 'type': 'number', 'label': 'Length', 'defaultValue': 4 },
      { 'name': 'snakeColor', 'type': 'color', 'label': 'Snake color', 'defaultValue': '#00ff00' },
      { 'name': 'foodColor', 'type': 'color', 'label': 'Food color', 'defaultValue': '#ff0000' }
    ],
  },
  {
    'label': 'Pong',
    'effect': 'pong',
    'fields': [
      { 'name': 'stepMs', 'type': 'number', 'label': 'Step (ms)', 'defaultValue': 45 },
      { 'name': 'paddleSize', 'type': 'number', 'label': 'Paddle size', 'defaultValue': 5 },
      { 'name': 'ballColor', 'type': 'color', 'label': 'Ball color', 'defaultValue': '#00ff00' },
      { 'name': 'paddleColor', 'type': 'color', 'label': 'Paddle color', 'defaultValue': '#ff0000' }
    ]
  },
  {
    'label': 'Bike',
    'effect': 'bike',
    'fields': [
      { 'name': 'mode', 'type': 'select', 'label': 'Mode', 'data': ['solid', 'blink', 'double'], 'defaultValue': 'double' },
      { 'name': 'brake', 'type': 'checkbox', 'label': 'Brake', 'defaultValue': false }
    ]
  }
]

export default EFFECTS;