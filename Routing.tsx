import React from "react"
import { createNativeStackNavigator } from '@react-navigation/native-stack';

import {
  useEnduserSession,
} from "@tellescope/react-components"

import { Home } from "./pages/Home"
import { Login } from "./pages/Login"
import { Register } from "./pages/Register"

type UnauthenticatedStackParamList = {
  Login: undefined;
  Register: undefined;
  Home: undefined;
};
const Stack = createNativeStackNavigator<UnauthenticatedStackParamList>();

export { NavigationContainer } from "@react-navigation/native"

export const AppNavigator = () => {
  const session = useEnduserSession()

  return (
    <Stack.Navigator>
      {!session.authToken ? (
        <>
          <Stack.Screen
            name="Login"
            component={Login}
            options={{
              title: 'Sign in',

              // When logging out, a pop animation feels intuitive
              // You can remove this if you want the default 'push' animation
              // animationTypeForReplace: state.isSignout ? 'pop' : 'push',
            }}
          />
          <Stack.Screen
            name="Register"
            component={Register}
            options={{ title: 'Register' }}
          />
        </>
      ) : (
        <>
          <Stack.Screen name="Home" component={Home} />
        </>
      )}
    </Stack.Navigator>
  )
}
