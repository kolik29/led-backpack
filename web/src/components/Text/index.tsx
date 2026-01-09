import { ColorPicker, Button, Checkbox, TextInput, NumberInput } from '@mantine/core';
import { useState } from 'react';
import { setText as sendText } from '../../Api/Requests';
import { QUICK_COLORS } from '../../storage/Colors';

const Text = () => {
  const [text, setText] = useState('');
  const [speed, setSpeed] = useState<string | number>(3);
  const [spacing, setSpacing] = useState<string | number>(0);
  const [scroll, setScroll] = useState(true);
  const [color, setColor] = useState('#ff0000');

  const send = async () => {
    const speedNum = typeof speed === "number" ? speed : 0;
    const spacingNum = typeof spacing === "number" ? spacing : 0;
    
    try {
      await sendText({
        text: text,
        color: color,
        speed: speedNum,
        scroll: scroll,
        spacing: spacingNum,
        y: 3
      });
    } catch (e) {
      console.error(e);
    }
  }

  return <>
    <div className='wrapper'>
      <TextInput
        label="Text"
        placeholder="Text"
        name="text"
        value={text}
        onChange={(e) => setText(e.target.value)}
      />
      <NumberInput
        label="Speed"
        placeholder="Speed"
        name="speed"
        value={speed}
        onChange={setSpeed}
      />
      <NumberInput
        label="Spacing"
        placeholder="Spacing"
        name="spacing"
        value={spacing}
        onChange={setSpacing}
      />
      <Checkbox
        defaultChecked
        label="Scroll"
        name="scroll"
        value={Number(scroll)}
        onChange={(e) => setScroll(e.target.checked ? true : false)}
      />
      <ColorPicker
        format="hex"
        value={color}
        onChange={setColor}
        swatches={QUICK_COLORS}
      />
      <Button
        variant="filled"
        onClick={send}
      >Send</Button>
    </div>
  </>
}

export default Text;