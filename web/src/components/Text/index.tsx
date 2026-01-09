import { ColorPicker, Button, Checkbox, TextInput } from '@mantine/core';
import { useState } from 'react';
import { setText as sendText } from '../../Api/Requests';
import { QUICK_COLORS } from '../../storage/Colors';

const Text = () => {
  const [text, setText] = useState('');
  const [speed, setSpeed] = useState('3');
  const [spacing, setSpacing] = useState('0');
  const [scroll, setScroll] = useState(true);
  const [color, setColor] = useState('#ff0000');

  const send = async () => {
    try {
      await sendText({
        text: text,
        color: color,
        speed: parseInt(speed),
        scroll: scroll,
        spacing: parseInt(spacing),
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
      <TextInput
        label="Speed"
        type="number"
        placeholder="Speed"
        name="speed"
        value={speed}
        onChange={(e) => setSpeed(e.target.value)}
      />
      <TextInput
        label="Spacing"
        type="number"
        placeholder="Spacing"
        name="spacing"
        value={spacing}
        onChange={(e) => setSpacing(e.target.value)}
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