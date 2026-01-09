import { useEffect, useLayoutEffect, useRef, useState } from 'react';
import Frame from '../../types/Frame';
import { drawFrame, emptyFrame } from '../../helpers/Frame';
import { ActionIcon, Button, ColorPicker, TextInput, Tooltip } from '@mantine/core';
import { IconPencil, IconEraser } from '@tabler/icons-react';
import styles from './index.module.sass';
import { sendFrame } from '../../Api/Requests';
import { QUICK_COLORS } from '../../storage/Colors';

const Editor = () => {
  const [frame, setFrame] = useState<Frame>(emptyFrame(16, 16));
  const [color, setColor] = useState('#ff0000');
  const [tool, setTool] = useState<'pencil' | 'eraser'>('pencil');

  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const drawingRef = useRef(false);

  const wrapRef = useRef<HTMLDivElement | null>(null);
  const [cell, setCell] = useState(16);

  useEffect(() => {
    if (!canvasRef.current) return;
    drawFrame(canvasRef.current, frame, true);
  }, [frame, cell]);

  const pointAt = (clientX: number, clientY: number) => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const rect = canvas.getBoundingClientRect();
    const cellX = rect.width / frame.w;
    const cellY = rect.height / frame.h;

    const x = Math.floor((clientX - rect.left) / cellX);
    const y = Math.floor((clientY - rect.top) / cellY);

    if (x < 0 || y < 0 || x >= frame.w || y >= frame.h) return;

    const i = y * frame.w + x;
    const nextColor = tool === 'eraser' ? '#000000' : color;

    if (frame.pixels[i] === nextColor) return;

    setFrame((prev) => {
      const pixels = prev.pixels.slice();
      pixels[i] = nextColor;
      return { ...prev, pixels };
    });
  };

  const onPointDown = (e: React.PointerEvent<HTMLCanvasElement>) => {
    drawingRef.current = true;
    (e.currentTarget as HTMLCanvasElement).setPointerCapture(e.pointerId);
    pointAt(e.clientX, e.clientY);
  }

  const onPointMove = (e: React.PointerEvent<HTMLCanvasElement>) => {
    if (!drawingRef.current) return;
    pointAt(e.clientX, e.clientY);
  }

  const onPointerUp = () => {
    drawingRef.current = false;
  }

  const clear = () => {
    setFrame(emptyFrame(frame.w, frame.h));
  }

  const send = async () => {
    try {
      await sendFrame(frame);
    } catch (e) {
      console.error(e);
    }
  }

  const resizeCanvas = () => {
    const wrap = wrapRef.current;
    const canvas = canvasRef.current;
    if (!wrap || !canvas) return;

    const cssW = wrap.clientWidth;
    if (cssW === 0) {
      requestAnimationFrame(resizeCanvas);
      return;
    }

    const cssH = Math.round(cssW * (frame.h / frame.w));

    const dpr = window.devicePixelRatio || 1;

    canvas.style.width = `${cssW}px`;
    canvas.style.height = `${cssH}px`;

    canvas.width = Math.round(cssW * dpr);
    canvas.height = Math.round(cssH * dpr);

    const ctx = canvas.getContext('2d');
    if (ctx) {
      ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
      ctx.imageSmoothingEnabled = false;
    }

    const cellSize = cssW / frame.w;
    setCell(cellSize);

    drawFrame(canvas, frame, true);
  };

  useEffect(() => {
    resizeCanvas();

    const wrap = wrapRef.current;
    if (!wrap) return;

    const ro = new ResizeObserver(() => resizeCanvas());
    ro.observe(wrap);

    window.addEventListener('resize', resizeCanvas);
    return () => {
      ro.disconnect();
      window.removeEventListener('resize', resizeCanvas);
    };
  }, [frame.w, frame.h]);

  useLayoutEffect(() => {
    resizeCanvas();
  }, [frame.w, frame.h]);

  return <>
    <div className="wrapper">
      <div className={styles.draw}>
        <Tooltip label="Pencil">
          <ActionIcon
            variant={tool === "pencil" ? "filled" : "light"}
            onClick={() => setTool("pencil")}
            size="lg"
          >
            <IconPencil size={18} />
          </ActionIcon>
        </Tooltip>

        <Tooltip label="Eraser">
          <ActionIcon
            variant={tool === "eraser" ? "filled" : "light"}
            onClick={() => setTool("eraser")}
            size="lg"
          >
            <IconEraser size={18} />
          </ActionIcon>
        </Tooltip>
      </div>

      <div
        ref={wrapRef}
        className="canvas"
        style={{ aspectRatio: `${frame.w} / ${frame.h}` }}
      >
        <canvas
          ref={canvasRef}
          onPointerDown={onPointDown}
          onPointerMove={onPointMove}
          onPointerUp={onPointerUp}
          onPointerCancel={onPointerUp}
          style={{ touchAction: "none" }}
        ></canvas>
      </div>
      
      <ColorPicker
        format="hex"
        value={color}
        onChange={setColor}
        swatches={QUICK_COLORS}
      />
      <TextInput value={color} onChange={e => setColor(e.target.value)}></TextInput>

      <div className={styles.actions}>
        <Button variant="light" onClick={clear}>Clear</Button>
        <Button onClick={send}>Send</Button>
      </div>
    </div>
  </>
}

export default Editor;