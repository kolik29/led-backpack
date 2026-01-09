import { Box, Button, Collapse, Group } from '@mantine/core'
import { useDisclosure } from '@mantine/hooks';
import styles from './index.module.sass';
import { setEffect } from '../../../Api/Requests';

const Effect = ({label = '', effect, children}: {label: string, effect: 'flashlight' | 'colorshift' | 'fire', children?: React.ReactNode}) => {
  const [opened, { toggle }] = useDisclosure(false);

  const onSubmit = (e: React.FormEvent<HTMLFormElement>) => {
    e.preventDefault();

    const formData = new FormData(e.currentTarget);
    const data = Object.fromEntries(formData);

    send(data);
  }

  const send = async (data: any) => {
    try {
      await setEffect({ effect: effect, params: data });
    } catch (e) {
      console.error(e);
    }
  }

  return <>
    <Box className={styles.box}>
      <Group>
        <div onClick={toggle} className={styles.effect}>{label}</div>
      </Group>
  
      <Collapse in={opened}>
        <form className={styles.content} onSubmit={onSubmit}>
          {children}
          <Button
            w="100%"
            variant="filled"
            type="submit"
          >Send</Button>
        </form>
      </Collapse>
    </Box>
  </>
}

export default Effect;